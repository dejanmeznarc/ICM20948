#include <Arduino.h>
#include <ICM20948/ICM20948.h>

#include "SPI.h"

SPIClass SPIsecond(PB15, PB14, PB13);


#define IMU_CS PA4
#define IMU_INT PA12


#define IMU_2_CS PB12
#define IMU_2_INT PA11

ICM20948 imu(IMU_CS, SPI, 7000000, IMU_INT);
ICM20948 imu2(IMU_2_CS, SPIsecond, 7000000, IMU_2_INT);


void checkStatus(ICM20948::status s, String str = "undefined error") {
    if (s != ICM20948::ok) {
        Serial.println("error at " + str + " code: " + String(s) + " msg:" + ICM20948::getErrMsg(s));
    }
}

void setup() {
    Serial.begin(1000000);
    Serial.println("Hello world!");

    for (int i = 0; i <= 7; ++i) {
        Serial.println(String(i) + " / 7 waiting...");
        delay(1000);
    }


    Serial.println("init of spi!");
    SPI.begin();


    Serial.println("init of imu");


    checkStatus(imu.begin(), "Begin");
    checkStatus(imu2.begin(), "Begin2");

    checkStatus(imu.read(), "read");
    checkStatus(imu2.read(), "read2");
}

void loop() {


    imu.read();
    imu2.read();
    Serial.println(String(imu.dataRaw.gyr.axis.x) + " -> " + String(imu.dataConverted.gyr.x)
                   + " | " + String(imu2.dataRaw.gyr.axis.x) + " -> " + String(imu2.dataConverted.gyr.x));


}