//
// Copyright (c) Dejan $originalComment.match("Copyright \(c\) (\d+)", 1, "-")2021.
//

#include "ICM20948.h"

////////////////////////////////////////////////////
/// PUBLIC METHODS                              ///
//////////////////////////////////////////////////

ICM20948::ICM20948(uint8_t pinCs, SPIClass &spiPort, uint32_t spiFreq, uint8_t pinInterrupt) {
    _spi = &spiPort;
    _spiSettings = SPISettings(spiFreq, MSBFIRST, SPI_MODE_0);
    _pin_cs = pinCs;
    _pin_int = pinInterrupt;
}

ICM20948::status ICM20948::begin(bool alsoConfigure) {
    status ret;

    pinMode(_pin_cs, OUTPUT);

    // Its NCS - negated chip select
    digitalWrite(_pin_cs, HIGH);

    // Kickstart SPI hardware
    _spi->begin();
    _spi->beginTransaction(_spiSettings);
    _spi->transfer(0x00);
    _spi->endTransaction();

    ret = checkWhoAmI();
    if (ret != ok) return ret;

    // reset device
    ret = reset();
    delay(50);
    if (ret != ok) return ret;

    // wake up
    ret = setSleep(false);
    if (ret != ok) return ret;

    // wake up
    ret = setLowPower(false);
    if (ret != ok) return ret;

    // wake up
    ret = setupMagnetometer(alsoConfigure);
    if (ret != ok) return ret;

    if (!alsoConfigure) return ret; // TODO: isnt up to user to configure sensor?

    return setDefaultConfig();
}

ICM20948::status ICM20948::read() {
    status ret;

    ret = readRawData();
    if (ret != ok) return ret;


    convertRawData();
    return ok;
}


ICM20948::status ICM20948::reset() {
    status ret;

    // Set correct bank
    ret = setBank(0);
    if (ret != ok) return ret;

    // Get previous settings from register, to change only required settings
    ICM_STRUCT_REG_PWR_MGMT_1_t reg;
    ret = regRead(ICM_REG_PWR_MGMT_1, (uint8_t *) &reg, sizeof(ICM_STRUCT_REG_PWR_MGMT_1_t));
    if (ret != ok) return ret;

    // change needed settings
    reg.DEVICE_RESET = 1;

    // write whole register back
    ret = regWrite(ICM_REG_PWR_MGMT_1, (uint8_t *) &reg, sizeof(ICM_STRUCT_REG_PWR_MGMT_1_t));
    if (ret != ok) return ret;

    return ok;
}

ICM20948::status ICM20948::resetMag() {
    uint8_t SRST = 1;
    // SRST: Soft reset
    // “0”: Normal
    // “1”: Reset
    // When “1” is set, all registers are initialized. After reset, SRST bit turns to “0” automatically.
    return i2cMasterSingleW(ICM_MAG_I2C_ADDR, ICM_MAG_REG_CNTL3, &SRST);
}

// General chip settings

ICM20948::status ICM20948::setLowPower(bool on) {
    status ret;

    // Set correct bank
    ret = setBank(0);
    if (ret != ok) return ret;

    // Get previous settings from register, to change only required settings
    ICM_STRUCT_REG_PWR_MGMT_1_t reg;
    ret = regRead(ICM_REG_PWR_MGMT_1, (uint8_t *) &reg, sizeof(ICM_STRUCT_REG_PWR_MGMT_1_t));
    if (ret != ok) return ret;

    // change needed settings
    reg.LP_EN = on ? 1 : 0;

    // write whole register back
    ret = regWrite(ICM_REG_PWR_MGMT_1, (uint8_t *) &reg, sizeof(ICM_STRUCT_REG_PWR_MGMT_1_t));
    if (ret != ok) return ret;

    return ok;
}

ICM20948::status ICM20948::setSleep(bool on) {
    status ret;

    // Set correct bank
    ret = setBank(0);
    if (ret != ok) return ret;

    // Get previous settings from register, to change only required settings
    ICM_STRUCT_REG_PWR_MGMT_1_t reg;
    ret = regRead(ICM_REG_PWR_MGMT_1, (uint8_t *) &reg, sizeof(ICM_STRUCT_REG_PWR_MGMT_1_t));
    if (ret != ok) return ret;

    // change needed settings
    reg.SLEEP = on ? 1 : 0;

    // write whole register back
    ret = regWrite(ICM_REG_PWR_MGMT_1, (uint8_t *) &reg, sizeof(ICM_STRUCT_REG_PWR_MGMT_1_t));
    if (ret != ok) return ret;

    return ok;
}

ICM20948::status ICM20948::setSampleMode(uint8_t sensors, uint8_t cnf_sample_mode) {

    if (!(sensors & (ICM_INTERNAL_ACC | ICM_INTERNAL_GYR | ICM_INTERNAL_MST))) return sensorNotSupported;

    status ret;

    // Set correct bank
    ret = setBank(0);
    if (ret != ok) return ret;

    // Get previous settings from register, to change only required settings
    ICM_STRUCT_REG_LP_CONFIG_t reg;
    ret = regRead(ICM_REG_LP_CONFIG, (uint8_t *) &reg, sizeof(ICM_STRUCT_REG_LP_CONFIG_t));
    if (ret != ok) return ret;

    // change needed settings

    if (sensors & ICM_INTERNAL_ACC) reg.ACCEL_CYCLE = cnf_sample_mode;
    if (sensors & ICM_INTERNAL_GYR) reg.GYRO_CYCLE = cnf_sample_mode;
    if (sensors & ICM_INTERNAL_MST) reg.I2C_MST_CYCLE = cnf_sample_mode;



    // write whole register back
    ret = regWrite(ICM_REG_LP_CONFIG, (uint8_t *) &reg, sizeof(ICM_STRUCT_REG_LP_CONFIG_t));
    if (ret != ok) return ret;

    return ok;

}

// interrupt settings
ICM20948::status ICM20948::rawDataInterrupt(void (*callback)()) {
    status ret;

    ret = setIntEnableOnRawDataReady(true);
    if (ret != ok) return ret;

    ret = setIntActiveLow(true);
    if (ret != ok) return ret;

    ret = setIntLatching(false);
    if (ret != ok) return ret;

    ret = setIntAnyRegReadClears(true);
    if (ret != ok) return ret;

    attachInterrupt(_pin_int, callback, FALLING);

    return ok;
}

ICM20948::status ICM20948::setIntEnableOnRawDataReady(bool on) {
    status ret;

    ret = setBank(0);
    if (ret != ok) return ret;

    ICM_STRUCT_REG_INT_ENABLE_1_t regIntEnable1;

    regIntEnable1.RAW_DATA_0_RDY_EN = on ? 1 : 0;
    regIntEnable1.reserved_0 = 0; // clear RAM garbage

    ret = regWrite(ICM_REG_INT_ENABLE_1, (uint8_t *) &regIntEnable1, sizeof(ICM_STRUCT_REG_INT_ENABLE_1_t));
    if (ret != ok) return ret;

    // wait 1us, so imu can write data
    delayMicroseconds(1);

    // read back same register to verify written;
    ret = regRead(ICM_REG_INT_ENABLE_1, (uint8_t *) &regIntEnable1, sizeof(ICM_STRUCT_REG_INT_ENABLE_1_t));
    if (ret != ok) return ret;

    if (regIntEnable1.RAW_DATA_0_RDY_EN != on) return err;

    return ok;
}

ICM20948::status ICM20948::setIntActiveLow(bool active_low_on) {
    status ret;

    // Set correct bank
    ret = setBank(0);
    if (ret != ok) return ret;

    // Get previous settings from register, to change only required settings
    ICM_STRUCT_REG_INT_PIN_CONF_t reg;
    ret = regRead(ICM_REG_INT_PIN_CONF, (uint8_t *) &reg, sizeof(ICM_STRUCT_REG_INT_PIN_CONF_t));
    if (ret != ok) return ret;

    // change needed settings
    reg.INT1_ACTL = active_low_on ? 1 : 0;

    // write whole register back
    ret = regWrite(ICM_REG_INT_PIN_CONF, (uint8_t *) &reg, sizeof(ICM_STRUCT_REG_INT_PIN_CONF_t));
    if (ret != ok) return ret;

    return ok;
}

ICM20948::status ICM20948::setIntLatching(bool en_latching) {
    status ret;

    // Set correct bank
    ret = setBank(0);
    if (ret != ok) return ret;

    // Get previous settings from register, to change only required settings
    ICM_STRUCT_REG_INT_PIN_CONF_t reg;
    ret = regRead(ICM_REG_INT_PIN_CONF, (uint8_t *) &reg, sizeof(ICM_STRUCT_REG_INT_PIN_CONF_t));
    if (ret != ok) return ret;

    // change needed settings
    reg.INT1_LATCH_EN = en_latching ? 1 : 0;

    // write whole register back
    ret = regWrite(ICM_REG_INT_PIN_CONF, (uint8_t *) &reg, sizeof(ICM_STRUCT_REG_INT_PIN_CONF_t));
    if (ret != ok) return ret;

    return ok;
}

ICM20948::status ICM20948::setIntAnyRegReadClears(bool on) {
    status ret;

    // Set correct bank
    ret = setBank(0);
    if (ret != ok) return ret;

    // Get previous settings from register, to change only required settings
    ICM_STRUCT_REG_INT_PIN_CONF_t reg;
    ret = regRead(ICM_REG_INT_PIN_CONF, (uint8_t *) &reg, sizeof(ICM_STRUCT_REG_INT_PIN_CONF_t));
    if (ret != ok) return ret;

    // change needed settings
    reg.INT_ANYRD_2CLEAR = on ? 1 : 0;

    // write whole register back
    ret = regWrite(ICM_REG_INT_PIN_CONF, (uint8_t *) &reg, sizeof(ICM_STRUCT_REG_INT_PIN_CONF_t));
    if (ret != ok) return ret;

    return ok;
}

uint8_t ICM20948::getIntPin() const {
    return _pin_int;
}


// Gyro configuration
ICM20948::status ICM20948::setGyrFss(uint8_t cnf_gyr_fss) {
    status ret;

    // Set correct bank
    ret = setBank(2);
    if (ret != ok) return ret;

    // Get previous settings from register, to change only required settings
    ICM_STRUCT_REG_GYR_CONF_1_t reg;
    ret = regRead(ICM_REG_GYR_CONF_1, (uint8_t *) &reg, sizeof(ICM_STRUCT_REG_GYR_CONF_1_t));
    if (ret != ok) return ret;

    // change needed settings
    reg.GYRO_FS_SEL = cnf_gyr_fss;

    // write whole register back
    ret = regWrite(ICM_REG_GYR_CONF_1, (uint8_t *) &reg, sizeof(ICM_STRUCT_REG_GYR_CONF_1_t));
    if (ret != ok) return ret;

    _gyr_fss = cnf_gyr_fss;

    return ok;
}

// this setting is active only if gyro sample mode is set to ICM_CNF_SAMPLE_MODE_CYCLE
// ODR (output data rate) is calculated with  1.1 kHz/(1+GYRO_SMPLRT_DIV[7:0])
ICM20948::status ICM20948::setGyrSampleRateDivider(uint8_t divider) {
    status ret;

    // Set correct bank
    ret = setBank(2);
    if (ret != ok) return ret;

    // write whole register back
    ret = regWrite(ICM_REG_GYR_SAMPLE_RATE_DIV, (uint8_t *) &divider);
    if (ret != ok) return ret;

    return ok;
}

ICM20948::status ICM20948::setGyrDlpfConf(uint8_t cnf_gyr_dlpf) {
    status ret;

    // Set correct bank
    ret = setBank(2);
    if (ret != ok) return ret;

    // Get previous settings from register, to change only required settings
    ICM_STRUCT_REG_GYR_CONF_1_t reg;
    ret = regRead(ICM_REG_GYR_CONF_1, (uint8_t *) &reg, sizeof(ICM_STRUCT_REG_GYR_CONF_1_t));
    if (ret != ok) return ret;

    // change needed settings
    reg.GYRO_DLPFCFG = cnf_gyr_dlpf;

    // write whole register back
    ret = regWrite(ICM_REG_GYR_CONF_1, (uint8_t *) &reg, sizeof(ICM_STRUCT_REG_GYR_CONF_1_t));
    if (ret != ok) return ret;

    return ok;
}

ICM20948::status ICM20948::setGyrDlpfEnabled(bool on) {
    status ret;

    // Set correct bank
    ret = setBank(2);
    if (ret != ok) return ret;

    // Get previous settings from register, to change only required settings
    ICM_STRUCT_REG_GYR_CONF_1_t reg;
    ret = regRead(ICM_REG_GYR_CONF_1, (uint8_t *) &reg, sizeof(ICM_STRUCT_REG_GYR_CONF_1_t));
    if (ret != ok) return ret;

    // change needed settings
    reg.GYRO_FCHOICE = on ? 1 : 0;

    // write whole register back
    ret = regWrite(ICM_REG_GYR_CONF_1, (uint8_t *) &reg, sizeof(ICM_STRUCT_REG_GYR_CONF_1_t));
    if (ret != ok) return ret;

    return ok;
}


// Accel config
ICM20948::status ICM20948::setAccFss(uint8_t cnf_acc_fss) {
    status ret;

    // Set correct bank
    ret = setBank(2);
    if (ret != ok) return ret;

    // Get previous settings from register, to change only required settings
    ICM_STRUCT_REG_ACC_CONF_1_t reg;
    ret = regRead(ICM_REG_ACC_CONF_1, (uint8_t *) &reg, sizeof(ICM_STRUCT_REG_ACC_CONF_1_t));
    if (ret != ok) return ret;

    // change needed settings
    reg.ACCEL_FS_SEL = cnf_acc_fss;

    // write whole register back
    ret = regWrite(ICM_REG_ACC_CONF_1, (uint8_t *) &reg, sizeof(ICM_STRUCT_REG_ACC_CONF_1_t));
    if (ret != ok) return ret;

    _acc_fss = cnf_acc_fss;

    return ok;
}

ICM20948::status ICM20948::setAccDlpfConf(uint8_t cnf_acc_dlpf) {
    status ret;

    // Set correct bank
    ret = setBank(2);
    if (ret != ok) return ret;

    // Get previous settings from register, to change only required settings
    ICM_STRUCT_REG_ACC_CONF_1_t reg;
    ret = regRead(ICM_REG_ACC_CONF_1, (uint8_t *) &reg, sizeof(ICM_STRUCT_REG_ACC_CONF_1_t));
    if (ret != ok) return ret;

    // change needed settings
    reg.ACCEL_DLPFCFG = cnf_acc_dlpf;

    // write whole register back
    ret = regWrite(ICM_REG_ACC_CONF_1, (uint8_t *) &reg, sizeof(ICM_STRUCT_REG_ACC_CONF_1_t));
    if (ret != ok) return ret;

    return ok;
}

ICM20948::status ICM20948::setAccDlpfEnabled(bool on) {
    status ret;

    // Set correct bank
    ret = setBank(2);
    if (ret != ok) return ret;

    // Get previous settings from register, to change only required settings
    ICM_STRUCT_REG_ACC_CONF_1_t reg;
    ret = regRead(ICM_REG_ACC_CONF_1, (uint8_t *) &reg, sizeof(ICM_STRUCT_REG_ACC_CONF_1_t));
    if (ret != ok) return ret;

    // change needed settings
    reg.ACCEL_FCHOICE = on ? 1 : 0;

    // write whole register back
    ret = regWrite(ICM_REG_ACC_CONF_1, (uint8_t *) &reg, sizeof(ICM_STRUCT_REG_ACC_CONF_1_t));
    if (ret != ok) return ret;

    return ok;
}



////////////////////////////////////////////////////
/// PRIVATE METHODS                             ///
//////////////////////////////////////////////////

// read and parse data

ICM20948::status ICM20948::readRawData() {
    status ret;

    // Set correct bank
    ret = setBank(0);
    if (ret != ok) return ret;

    // read
    const uint8_t numbytes = 14 + 9; //Read Accel, gyro, temp, and 9 bytes of mag
    uint8_t buff[numbytes];

    ret = regRead(ICM_REG_ACCEL_XOUT_H, buff, numbytes);
    if (ret != ok) return ret;

    dataRaw.acc.axis.x = (int16_t) ((buff[0] << 8) | (buff[1] & 0xFF));
    dataRaw.acc.axis.y = (int16_t) ((buff[2] << 8) | (buff[3] & 0xFF));
    dataRaw.acc.axis.z = (int16_t) ((buff[4] << 8) | (buff[5] & 0xFF));

    dataRaw.gyr.axis.x = (int16_t) ((buff[6] << 8) | (buff[7] & 0xFF));
    dataRaw.gyr.axis.y = (int16_t) ((buff[8] << 8) | (buff[9] & 0xFF));
    dataRaw.gyr.axis.z = (int16_t) ((buff[10] << 8) | (buff[11] & 0xFF));

    dataRaw.tmp.val = (int16_t) ((buff[12] << 8) | (buff[13] & 0xFF));

    dataRaw.magStat1 = buff[14];
    dataRaw.mag.axis.x = (int16_t) ((buff[16] << 8) | (buff[15] & 0xFF)); //Mag data is read little endian
    dataRaw.mag.axis.y = (int16_t) ((buff[18] << 8) | (buff[17] & 0xFF));
    dataRaw.mag.axis.z = (int16_t) ((buff[20] << 8) | (buff[19] & 0xFF));
    dataRaw.magStat2 = buff[22];

    return ok;
}

void ICM20948::convertRawData() {
    dataConverted.gyr.x = getGyrDPS(dataRaw.gyr.axis.x);
    dataConverted.gyr.y = getGyrDPS(dataRaw.gyr.axis.y);
    dataConverted.gyr.z = getGyrDPS(dataRaw.gyr.axis.z);

    dataConverted.acc.x = getAccMG(dataRaw.acc.axis.x);
    dataConverted.acc.y = getAccMG(dataRaw.acc.axis.y);
    dataConverted.acc.z = getAccMG(dataRaw.acc.axis.z);

    dataConverted.mag.x = getMagUT(dataRaw.mag.axis.x);
    dataConverted.mag.y = getMagUT(dataRaw.mag.axis.y);
    dataConverted.mag.z = getMagUT(dataRaw.mag.axis.z);

    dataConverted.temp = getTempC(dataRaw.tmp.val);

    // TODO: mag accruacy
}

double ICM20948::getGyrDPS(int16_t raw) const {
    switch (_gyr_fss) {
        case ICM_CNF_GYR_FSS_DPS250:
            return (((double) raw) / 131);
            break;
        case ICM_CNF_GYR_FSS_DPS500:
            return (((double) raw) / 65.5);
            break;
        case ICM_CNF_GYR_FSS_DPS1000:
            return (((double) raw) / 32.8);
            break;
        case ICM_CNF_GYR_FSS_DPS2000:
            return (((double) raw) / 16.4);
            break;
        default:
            return 0;
            break;
    }
}

double ICM20948::getAccMG(int16_t raw) const {
    switch (_acc_fss) {
        case ICM_CNF_ACC_FSS_GMP2:
            return (((double) raw) / 16.384);
            break;
        case ICM_CNF_ACC_FSS_GMP4:
            return (((double) raw) / 8.192);
            break;
        case ICM_CNF_ACC_FSS_GMP8:
            return (((double) raw) / 4.096);
            break;
        case ICM_CNF_ACC_FSS_GMP16:
            return (((double) raw) / 2.048);
            break;
        default:
            return 0;
            break;
    }
}

double ICM20948::getMagUT(int16_t raw) {
    return (((double) raw) * 0.15);
}

double ICM20948::getTempC(int16_t raw) {
    return (((double) raw - 21) / 333.87) + 21;
}

// who am i

ICM20948::status ICM20948::checkWhoAmI() {

    uint8_t whoami = 0x00;

    setBank(0);
    regRead(ICM_REG_WHO_AM_I, &whoami);

    if (whoami != ICM_WHOAMI) return wrongID;

    return ok;
}

ICM20948::status ICM20948::checkMagWhoAmI() {
    status ret;

    uint8_t whoiam1, whoiam2;

    ret = regReadMag(ICM_MAG_REG_WHOAMI1, &whoiam1);
    if (ret != ok) return ret;

    ret = regReadMag(ICM_MAG_REG_WHOAMI2, &whoiam2);
    if (ret != ok) return ret;


    if ((whoiam1 == (ICM_MAG_WHOAMI >> 8)) && (whoiam2 == (ICM_MAG_WHOAMI & 0xFF))) {
        return ok;
    }
    return wrongID;
}

ICM20948::status ICM20948::setupMagnetometer(bool alsoConfigure) {
    status ret;


    ret = i2cMasterSetEnable(true);
    if (ret != ok) return ret;

    ret = resetMag();
    if (ret != ok) return ret;



    //After a ICM reset the Mag sensor may stop responding over the I2C master
    //Reset the Master I2C until it respond
    uint8_t tries = 0;
    while (tries < 10) {
        tries++;

        //See if we can read the WhoIAm register correctly
        if (checkMagWhoAmI() == ok) break; //WIA matched!

        ret = i2cMasterReset();
        if (ret != ok) return ret;

        delay(10);

    }

    if (tries >= 10) return wrongID;

    //Return now if minimal is true. The mag will be configured manually for the DMP
    if (!alsoConfigure) return ok;


    ICM_MAG_STRUCT_REG_CNTL2_t reg;
    reg.MODE = ICM_MAG_MODE_CONT_100HZ;
    reg.reserved_0 = 0; // Make sure the unused bits are clear. Probably redundant, but prevents confusion when looking at the I2C traffic

    ret = regWriteMag(ICM_MAG_REG_CNTL2, (uint8_t *) &reg);
    if (ret != ok) return ret;


    ret = i2cControllerConfigure(0, ICM_MAG_I2C_ADDR, ICM_MAG_REG_ST1, 9, true, true, false, false, false, 0);
    if (ret != ok) return ret;


    return ok;
}

ICM20948::status ICM20948::setDefaultConfig() {
    status ret;
    // options: ICM_20948_Sample_Mode_Continuous or ICM_20948_Sample_Mode_Cycled
    ret = setSampleMode((ICM_INTERNAL_ACC | ICM_INTERNAL_GYR), ICM_CNF_SAMPLE_MODE_CONT);
    if (ret != ok) return ret;

    // Configure gyro
    ret = setGyrFss(ICM_CNF_GYR_FSS_DPS250);
    if (ret != ok) return ret;
    ret = setGyrDlpfConf(ICM_CNF_GYR_DLPF_D361BW4_N376BW5);
    if (ret != ok) return ret;
    ret = setGyrDlpfEnabled(false);
    if (ret != ok) return ret;


    //Configure accel
    ret = setAccFss(ICM_CNF_ACC_FSS_GMP2);
    if (ret != ok) return ret;
    ret = setAccDlpfConf(ICM_CNF_ACC_DLPF_D473BW_N499BW);
    if (ret != ok) return ret;
    ret = setAccDlpfEnabled(false);
    if (ret != ok) return ret;

    return ok;
}


// Read/write functions to registers

ICM20948::status ICM20948::setBank(uint8_t bank) {
    if (bank > 3) return paramErr;

    if (bank == _cur_bank) return ok;

    bank = (bank << 4) & 0x30; // bits 5:4 of REG_BANK_SEL

    return regWrite(ICM_REG_BANK_SELECT, &bank, 1);
}

ICM20948::status ICM20948::regRead(uint8_t reg, uint8_t *data, uint32_t len) {

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

ICM20948::status ICM20948::regReadMag(uint8_t reg, uint8_t *data) {
    return i2cMasterSingleR(ICM_MAG_I2C_ADDR, reg, data);
}

ICM20948::status ICM20948::regWrite(uint8_t reg, const uint8_t *data, uint32_t len) {

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

ICM20948::status ICM20948::regWriteMag(uint8_t reg, uint8_t *data) {
    return i2cMasterSingleW(ICM_MAG_I2C_ADDR, reg, data);
}


// I2c config for access of magnetometer

ICM20948::status ICM20948::i2cMasterSetEnable(bool enable) {
    status ret;

    ret = i2cMasterSetPassthrough(false);
    if (ret != ok) return ret;


    // configure master
    ret = setBank(3);
    if (ret != ok) return ret;

    ICM_STRUCT_REG_I2C_MST_CTRL_t ctrl;
    ret = regRead(ICM_REG_I2C_MST_CTRL, (uint8_t *) &ctrl, sizeof(ICM_STRUCT_REG_I2C_MST_CTRL_t));
    if (ret != ok) return ret;

    ctrl.I2C_MST_CLK = 0x07; // corresponds to 345.6 kHz, good for up to 400 kHz
    ctrl.I2C_MST_P_NSR = 1;

    ret = regWrite(ICM_REG_I2C_MST_CTRL, (uint8_t *) &ctrl, sizeof(ICM_STRUCT_REG_I2C_MST_CTRL_t));
    if (ret != ok) return ret;


    //enable master
    ret = setBank(0);
    if (ret != ok) return ret;

    ICM_STRUCT_REG_USER_CTRL_t reg;
    ret = regRead(ICM_REG_USER_CTRL, (uint8_t *) &reg, sizeof(ICM_STRUCT_REG_USER_CTRL_t));
    if (ret != ok) return ret;

    reg.I2C_MST_EN = enable ? 1 : 0;

    ret = regWrite(ICM_REG_USER_CTRL, (uint8_t *) &reg, sizeof(ICM_STRUCT_REG_USER_CTRL_t));
    if (ret != ok) return ret;


    return ICM20948::ok;
}

ICM20948::status ICM20948::i2cMasterReset() {

    status ret;

    // Set correct bank
    ret = setBank(0);
    if (ret != ok) return ret;

    // Get previous settings from register, to change only required settings
    ICM_STRUCT_REG_USER_CTRL_t reg;
    ret = regRead(ICM_REG_USER_CTRL, (uint8_t *) &reg, sizeof(ICM_STRUCT_REG_USER_CTRL_t));
    if (ret != ok) return ret;

    // change needed settings
    reg.I2C_MST_RST = 1; //Reset!

    // write whole register back
    ret = regWrite(ICM_REG_USER_CTRL, (uint8_t *) &reg, sizeof(ICM_STRUCT_REG_USER_CTRL_t));
    if (ret != ok) return ret;

    return ok;


}

ICM20948::status ICM20948::i2cMasterSetPassthrough(bool passthrough) {
    status ret;

    // Set correct bank
    ret = setBank(0);
    if (ret != ok) return ret;

    // Get previous settings from register, to change only required settings
    ICM_STRUCT_REG_INT_PIN_CNFG_t reg;
    ret = regRead(ICM_REG_PWR_MGMT_1, (uint8_t *) &reg, sizeof(ICM_STRUCT_REG_INT_PIN_CNFG_t));
    if (ret != ok) return ret;

    // change needed settings
    reg.BYPASS_EN = passthrough ? 1 : 0;

    // write whole register back
    ret = regWrite(ICM_REG_PWR_MGMT_1, (uint8_t *) &reg, sizeof(ICM_STRUCT_REG_INT_PIN_CNFG_t));
    if (ret != ok) return ret;

    return ok;
}

ICM20948::status ICM20948::i2cMasterSingleW(uint8_t addr, uint8_t reg, uint8_t *data) {
    return i2cControllerTransaction(addr, reg, data, 1, false, true);
}

ICM20948::status ICM20948::i2cMasterSingleR(uint8_t addr, uint8_t reg, uint8_t *data) {
    return i2cControllerTransaction(addr, reg, data, 1, true, true);
}


ICM20948::status
ICM20948::i2cControllerTransaction(uint8_t addr, uint8_t reg, uint8_t *data, int len, bool Rw, bool sendRegAddr) {
    status ret;

    addr = (((Rw) ? 0x80 : 0x00) | addr);


    ret = setBank(3);
    if (ret != ok) return ret;
    regWrite(ICM_REG_I2C_SLV4_ADDR, (uint8_t *) &addr);
    if (ret != ok) return ret;

    ret = setBank(3);
    if (ret != ok) return ret;
    regWrite(ICM_REG_I2C_SLV4_REG, (uint8_t *) &reg);
    if (ret != ok) return ret;


    ICM_STRUCT_REG_I2C_SLV4_CTRL_t i2cControl;
    i2cControl.EN = 1;
    i2cControl.INT_EN = false;
    i2cControl.DLY = 0;
    i2cControl.REG_DIS = !sendRegAddr;


    ICM_STRUCT_REG_I2C_SLV4_DI_t i2cMasterStatus;
    bool txnFailed = false;
    uint16_t nByte = 0;

    while (nByte < len) {

        if (!Rw) {
            ret = setBank(3);
            if (ret != ok) return ret;
            regWrite(ICM_REG_I2C_SLV4_DO, (uint8_t *) &(data[nByte]));
            if (ret != ok) return ret;
        }

        // kick off tnx
        ret = setBank(3);
        if (ret != ok) return ret;
        regWrite(ICM_REG_I2C_SLV4_CTRL, (uint8_t *) &i2cControl);
        if (ret != ok) return ret;

        // long tsTimeout = millis() + 3000;  // Emergency timeout for txn (hard coded to 3 secs)
        uint32_t max_cycles = 1000;
        uint32_t count = 0;
        bool peripheral4Done = false;
        while (!peripheral4Done) {
            ret = setBank(0);
            if (ret != ok) return ret;

            regRead(ICM_REG_I2C_SLV4_DI, (uint8_t *) &i2cMasterStatus);
            if (ret != ok) return ret;

            peripheral4Done = (i2cMasterStatus.I2C_PERIPH4_DONE /*| (millis() > tsTimeout) */); //Avoid forever-loops
            peripheral4Done |= (count >= max_cycles);
            count++;
        }
        txnFailed = (i2cMasterStatus.I2C_PERIPH4_NACK /*| (millis() > tsTimeout) */);
        txnFailed |= (count >= max_cycles);
        if (txnFailed) break;


        if (Rw) {
            ret = setBank(3);
            if (ret != ok) return ret;
            regRead(ICM_REG_I2C_SLV4_DI, &data[nByte]);
            if (ret != ok) return ret;
        }
        nByte++;
    }


    if (txnFailed) return err;

    return ok;
}

ICM20948::status
ICM20948::i2cControllerConfigure(uint8_t slaveNum, uint8_t addr, uint8_t reg, uint8_t len, bool Rw, bool enable,
                                 bool data_only, bool grp, bool swap, uint8_t dataOut) {
    status ret;

    uint8_t periph_addr_reg;
    uint8_t periph_reg_reg;
    uint8_t periph_ctrl_reg;
    uint8_t periph_do_reg;

    switch (slaveNum) {
        case 0:
            periph_addr_reg = ICM_REG_I2C_SLV0_ADDR;
            periph_reg_reg = ICM_REG_I2C_SLV0_REG;
            periph_ctrl_reg = ICM_REG_I2C_SLV0_CTRL;
            periph_do_reg = ICM_REG_I2C_SLV0_DO;
            break;
        case 1:
            periph_addr_reg = ICM_REG_I2C_SLV1_ADDR;
            periph_reg_reg = ICM_REG_I2C_SLV1_REG;
            periph_ctrl_reg = ICM_REG_I2C_SLV1_CTRL;
            periph_do_reg = ICM_REG_I2C_SLV1_DO;
            break;
        case 2:
            periph_addr_reg = ICM_REG_I2C_SLV2_ADDR;
            periph_reg_reg = ICM_REG_I2C_SLV2_REG;
            periph_ctrl_reg = ICM_REG_I2C_SLV2_CTRL;
            periph_do_reg = ICM_REG_I2C_SLV2_DO;
            break;
        case 3:
            periph_addr_reg = ICM_REG_I2C_SLV3_ADDR;
            periph_reg_reg = ICM_REG_I2C_SLV3_REG;
            periph_ctrl_reg = ICM_REG_I2C_SLV3_CTRL;
            periph_do_reg = ICM_REG_I2C_SLV3_DO;
            break;
        default:
            return paramErr;
    }

    // Set correct bank
    ret = setBank(3);
    if (ret != ok) return ret;

    ICM_STRUCT_I2C_SLV_ADDR_t address;
    address.ID = addr;
    address.RNW = Rw ? 1 : 0;


    ret = regWrite(periph_addr_reg, (uint8_t *) &address, sizeof(ICM_STRUCT_I2C_SLV_ADDR_t));
    if (ret != ok) return ret;

    // If we are setting up a write, configure the Data Out register too
    if (!Rw) {
        ICM_STRUCT_REG_I2C_SLV_DO_t dataOutByte;
        dataOutByte.DO = dataOut;

        ret = regWrite(periph_do_reg, (uint8_t *) &dataOutByte, sizeof(ICM_STRUCT_REG_I2C_SLV_DO_t));
        if (ret != ok) return ret;
    }

    // Set the peripheral sub-address (register address)
    ICM_STRUCT_REG_I2C_SLV_REG_t subAddress;
    subAddress.REG = reg;

    ret = regWrite(periph_reg_reg, (uint8_t *) &subAddress, sizeof(ICM_STRUCT_REG_I2C_SLV_REG_t));
    if (ret != ok) return ret;

    // Set up the control info
    ICM_STRUCT_REG_I2C_SLV_CTRL_t ctrl;
    ctrl.LENG = len;
    ctrl.EN = enable;
    ctrl.REG_DIS = data_only;
    ctrl.GRP = grp;
    ctrl.BYTE_SW = swap;
    ret = regWrite(periph_ctrl_reg, (uint8_t *) &ctrl, sizeof(ICM_STRUCT_REG_I2C_SLV_CTRL_t));
    if (ret != ok) return ret;

    return ok;
}

String ICM20948::getErrMsg(ICM20948::status s) {
    switch (s) {
        case ok:
            return "All is well.";
            break;
        case err:
            return "General Error";
            break;
        case notImpl:
            return "Not Implemented";
            break;
        case paramErr:
            return "Parameter Error";
            break;
        case wrongID:
            return "Wrong ID";
            break;
        case invalSensor:
            return "Invalid Sensor";
            break;
        case noData:
            return "Data Underflow";
            break;
        case sensorNotSupported:
            return "Sensor Not Supported";
            break;
        case DMPNotSupported:
            return "DMP Firmware Not Supported. Is #define ICM_20948_USE_DMP commented in util/ICM_20948_C.h?";
            break;
        case DMPVerifyFail:
            return "DMP Firmware Verification Failed";
            break;
        case FIFONoDataAvail:
            return "No FIFO Data Available";
            break;
        case FIFOIncompleteData:
            return "DMP data in FIFO was incomplete";
            break;
        case FIFOMoreDataAvail:
            return "More FIFO Data Available";
            break;
        case unrecognisedDMPHeader:
            return "Unrecognised DMP Header";
            break;
        case unrecognisedDMPHeader2:
            return "Unrecognised DMP Header2";
            break;
        case invalDMPRegister:
            return "Invalid DMP Register";
            break;
        default:
            return "Unknown Status code";
            break;
    }
}






















