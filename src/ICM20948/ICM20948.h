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


    explicit ICM20948(uint8_t pinCs, SPIClass &spiPort = SPI, uint32_t spiFreq = 7000000);


    ICM20948::status begin(bool alsoConfigure);


private:


    uint8_t _pin_cs;
    SPIClass *_spi;
    SPISettings _spiSettings;


    uint8_t _cur_bank = 255; // bank 255 doesnt exists, set it as init


    status checkWhoAmI();

    status reset();

    status setSleep(bool on);

    status read(uint8_t reg, uint8_t *data, uint32_t len = 1);

    status write(uint8_t reg, uint8_t *data, uint32_t len = 1);

    status setBank(uint8_t bank);


    status setLowPower(bool on);

    ICM20948::status setupMagnetometer(bool alsoConfigure);

    status setI2cMasterPassthrough(bool passthrough);

    status setI2cMasterEnable(bool enable);

    status resetMag();

    ICM20948::status i2cMasterSingleW(uint8_t addr, uint8_t reg, uint8_t *data);

    ICM20948::status
    i2cControllerTransaction(uint8_t addr, uint8_t reg, uint8_t *data, int len, bool Rw, bool sendRegAddr);

    ICM20948::status i2cMasterSingleR(uint8_t addr, uint8_t reg, uint8_t *data);

    status checkMagWhoAmI();

    ICM20948::status readMag(uint8_t reg, uint8_t *data);

    status resetI2cMaster();

    status writeMag(uint8_t reg, uint8_t *data);

    ICM20948::status
    i2cControllerConfigure(uint8_t slaveNum, uint8_t addr, uint8_t reg, uint8_t len, bool Rw, bool enable,
                           bool data_only, bool grp, bool swap, uint8_t dataOut);

    status setSampleMode(uint8_t sensors, uint8_t cnf_sample_mode);

    status setGyrFSS(uint8_t cnf_gyr_fss);

    status setAccFSS(uint8_t cnf_acc_fss);
};


#endif //ICM20948_ICM20948_H
