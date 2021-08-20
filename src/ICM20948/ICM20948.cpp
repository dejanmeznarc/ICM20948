//
// Created by dejan on 20/08/2021.
//

#include "ICM20948.h"

ICM20948::ICM20948(uint8_t pinCs, SPIClass &spiPort, uint32_t spiFreq) {
    _spi = &spiPort;
    _spiSettings = SPISettings(spiFreq, MSBFIRST, SPI_MODE_0);
    _pin_cs = pinCs;
}

ICM20948::status ICM20948::begin() {
    status ret;

    pinMode(_pin_cs, OUTPUT);

    // Its NCS - negated chip select
    digitalWrite(_pin_cs, HIGH);

    // Kickstart SPI hardware
    _spi->beginTransaction(_spiSettings);
    _spi->transfer(0x00);
    _spi->endTransaction();

    ret = checkWhoAmI();
    if (ret != ok) return ret;

    // reset device
    ret = reset();
    delay(50);
    if (ret != ok) return ret;

    return ok;
}


ICM20948::status ICM20948::checkWhoAmI() {

    uint8_t whoami = 0x00;

    setBank(0);
    read(ICM_REG_WHO_AM_I, &whoami);

    if (whoami != ICM_WHOAMI) return wrongID;

    return ok;
}


ICM20948::status ICM20948::read(uint8_t reg, uint8_t *data, uint32_t len) {

    // 'Kickstart' the SPI hardware. This is a fairly high amount of overhead,
    // but it guarantees that the lines will start in the correct states even
    // when sharing the SPI bus with devices that use other modes
    _spi->beginTransaction(_spiSettings);
    _spi->transfer(0x00);
    _spi->endTransaction();


    _spi->beginTransaction(_spiSettings);
    digitalWrite(_pin_cs, LOW);
    //   delayMicroseconds(5);
    _spi->transfer(((reg & 0x7F) | 0x80));
    //  SPI.transfer(data, len); // Can't do this thanks to Arduino's stupid implementation
    for (uint32_t i = 0; i < len; i++) {
        *(data + i) = _spi->transfer(0x00);
    }
    //   delayMicroseconds(5);
    digitalWrite(_pin_cs, HIGH);
    _spi->endTransaction();

    return ok;
}

ICM20948::status ICM20948::write(uint8_t reg, uint8_t *data, uint32_t len) {

    // 'Kickstart' the SPI hardware. This is a fairly high amount of overhead,
    // but it guarantees that the lines will start in the correct states even
    // when sharing the SPI bus with devices that use other modes
    _spi->beginTransaction(_spiSettings);
    _spi->transfer(0x00);
    _spi->endTransaction();

    // Start procedure
    _spi->beginTransaction(_spiSettings);
    // Select device
    digitalWrite(_pin_cs, LOW);
    // Tell what register to write to
    _spi->transfer((reg & 0x7F) | 0x00);
    // Send all data
    for (uint32_t i = 0; i < len; ++i) {
        _spi->transfer(*(data + i));
    }
    // unselect device and stop
    digitalWrite(_pin_cs, HIGH);
    _spi->endTransaction();

    return ok;
}

ICM20948::status ICM20948::setBank(uint8_t bank) {
    if (bank > 3) return paramErr;

    if (bank == _cur_bank) return ok;

    bank = (bank << 4) & 0x30; // bits 5:4 of REG_BANK_SEL

    return write(ICM_REG_BANK_SELECT, &bank, 1);
}

ICM20948::status ICM20948::reset() {
    status ret;

    // Set correct bank
    ret = setBank(0);
    if (ret != ok) return ret;

    // Get previous settings from register, to change only required settings
    ICM_STRUCT_REG_PWR_MGMT_1_t reg;
    ret = read(ICM_REG_PWR_MGMT_1, (uint8_t *) &reg, sizeof(ICM_STRUCT_REG_PWR_MGMT_1_t));
    if (ret != ok) return ret;

    // change needed settings
    reg.DEVICE_RESET = 1;

    // write whole register back
    ret = write(ICM_REG_PWR_MGMT_1, (uint8_t *) &reg, sizeof(ICM_STRUCT_REG_PWR_MGMT_1_t));
    if (ret != ok) return ret;

    return ok;
}
