#include <Arduino.h>
#include <ICM20948/ICM20948.h>

#include "SPI.h"

#define IMU_CS PA4
#define IMU_INT PA12


ICM20948 imu(IMU_CS, SPI, 7000000, IMU_INT);


void checkStatus(ICM20948::status s, String str = "undefined error") {
    Serial.println(s == ICM20948::ok ? str + ": ok" : " error: " + String(s));
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

    checkStatus(imu.read(), "read");
}

void loop() {


    imu.read();
    Serial.println(String(imu.dataRaw.gyr.axis.x) + " -> " + String(imu.dataConverted.gyr.x));

}