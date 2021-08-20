//
// Created by dejan on 20/08/2021.
//

#ifndef ICM20948_ICM20948_H
#define ICM20948_ICM20948_H

#include "Arduino.h"
#include "SPI.h"
#include "register_map.h"




class ICM20948 {

public:

    enum status {
        ok = 0x00, // The only return code that means all is well
        err,       // A general error
        notImpl,   // Returned by virtual functions that are not implemented
        paramErr,
        wrongID,
        invalSensor, // Tried to apply a function to a sensor that does not support it (e.g. DLPF to the temperature sensor)
        noData,
        sensorNotSupported,
        DMPNotSupported,    // DMP not supported (no #define ICM_20948_USE_DMP)
        DMPVerifyFail,      // DMP was written but did not verify correctly
        FIFONoDataAvail,    // FIFO contains no data
        FIFOIncompleteData, // FIFO contained incomplete data
        FIFOMoreDataAvail,  // FIFO contains more data
        unrecognisedDMPHeader,
        unrecognisedDMPHeader2,
        invalDMPRegister, // Invalid DMP Register

        NUM,
        unknown,
    };


    ICM20948(uint8_t pinCs, SPIClass &spiPort = SPI, uint32_t spiFreq = 7000000) {
        _spi = &spiPort;
        _spiSettings = SPISettings(spiFreq, MSBFIRST, SPI_MODE_0);
        _pin_cs = pinCs;
    }


    status begin() {

        pinMode(_pin_cs, OUTPUT);

        // Its NCS - negated chip select
        digitalWrite(_pin_cs, HIGH);

        // Kickstart SPI hardware
        _spi->beginTransaction(_spiSettings);
        _spi->transfer(0x00);
        _spi->endTransaction();

        // Check Who am i
        uint8_t whoami = 0x00;
        setBank(0);
        status tempStatus = read(ICM_REG_WHO_AM_I, &whoami);
        if(tempStatus != ok) return tempStatus;
        if(whoami != ICM_WHOAMI) return wrongID;

        return ok;
    }


    status read(uint8_t reg, uint8_t *data, uint32_t len = 1) {

        if (_spi == nullptr) return paramErr;

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

    status write(uint8_t reg, uint8_t *data, uint32_t len = 1) {

        if (_spi == nullptr) return paramErr;

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


private:


    uint8_t _pin_cs;
    SPIClass *_spi;
    SPISettings _spiSettings;


    uint8_t _cur_bank;

    status setBank(uint8_t bank) {
        if (bank > 3) return paramErr;

        if (bank == _cur_bank) return ok;

        bank = (bank << 4) & 0x30; // bits 5:4 of REG_BANK_SEL

        return write(ICM_REG_BANK_SELECT, &bank, 1);
    }


};


#endif //ICM20948_ICM20948_H
