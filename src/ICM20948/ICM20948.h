//
// Copyright (c) Dejan $originalComment.match("Copyright \(c\) (\d+)", 1, "-")2021.
//




#ifndef ICM20948_ICM20948_H
#define ICM20948_ICM20948_H

#include "Arduino.h"
#include "SPI.h"
#include "includes/registers/all.h"
#include "includes/datatypes.h"


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


    ICM_data_raw_t dataRaw{};
    ICM_data_converted_t dataConverted{};


    explicit ICM20948(uint8_t pinCs, SPIClass &spiPort = SPI, uint32_t spiFreq = 7000000, uint8_t pinInterrupt = 0);


    status begin(bool alsoConfigure = true);

    status read();


    // Chip resets
    status reset();

    status resetMag();


    // General chip settings
    status setLowPower(bool on);

    status setSleep(bool on);

    status setSampleMode(uint8_t sensors, uint8_t cnf_sample_mode);


    // Interrupt settings
    status rawDataInterrupt(void (*callback)());

    status setIntEnableOnRawDataReady(bool on);

    status setIntActiveLow(bool active_low_on);

    status setIntLatching(bool en_latching);// If not latching then the interrupt is a 50 us pulse
    // If enabled, *ANY* read will clear the INT_STATUS register. So if you have multiple interrupt sources enabled be sure to read INT_STATUS first
    status setIntAnyRegReadClears(bool on);

    uint8_t getIntPin() const;


    // Gyro configuration
    status setGyrFss(uint8_t cnf_gyr_fss);

    status setGyrSampleRateDivider(uint8_t divider);

    status setGyrDlpfConf(uint8_t cnf_gyr_dlpf);

    status setGyrDlpfEnabled(bool on);


    // Accel config
    status setAccFss(uint8_t cnf_acc_fss);

    status setAccDlpfConf(uint8_t cnf_acc_dlpf);

    status setAccDlpfEnabled(bool on);


    static String getErrMsg(status s);


private:


    uint8_t _pin_cs;
    uint8_t _pin_int;
    SPIClass *_spi;
    SPISettings _spiSettings;

    uint8_t _gyr_fss = 255;
    uint8_t _acc_fss = 255;
    uint8_t _cur_bank = 255; // bank 255 doesnt exists, set it as init


    status readRawData();

    void convertRawData();

    double getGyrDPS(int16_t raw) const;

    double getAccMG(int16_t raw) const;

    static double getTempC(int16_t raw);

    static double getMagUT(int16_t raw);

    // other functions that set some settings
    status checkWhoAmI();

    status checkMagWhoAmI();

    status setupMagnetometer(bool alsoConfigure);

    status setDefaultConfig();


    // Read and write functions to registers
    status setBank(uint8_t bank);

    status regRead(uint8_t reg, uint8_t *data, uint32_t len = 1);

    status regReadMag(uint8_t reg, uint8_t *data);

    status regWrite(uint8_t reg, const uint8_t *data, uint32_t len = 1);

    status regWriteMag(uint8_t reg, uint8_t *data);


    // I2C configuration for accessing mag
    status i2cMasterSetEnable(bool enable);

    status i2cMasterReset();

    status i2cMasterSetPassthrough(bool passthrough);

    status i2cMasterSingleW(uint8_t addr, uint8_t reg, uint8_t *data);

    status i2cMasterSingleR(uint8_t addr, uint8_t reg, uint8_t *data);


    status i2cControllerConfigure(uint8_t slaveNum, uint8_t addr, uint8_t reg, uint8_t len, bool Rw, bool enable,
                                  bool data_only, bool grp, bool swap, uint8_t dataOut);

    status i2cControllerTransaction(uint8_t addr, uint8_t reg, uint8_t *data, int len, bool Rw, bool sendRegAddr);


};


#endif //ICM20948_ICM20948_H
