#include <Arduino.h>
#include <ICM20948/ICM20948.h>

#include "SPI.h"

#define IMU_CS PA4
#define IMU_INT PA12


ICM20948 imu(IMU_CS, SPI, 7000000);


ICM20948::status last_status = ICM20948::unknown;


void setup() {
    Serial.begin(1000000);
    Serial.println("Hello world!");

    for (int i = 0; i <= 7; ++i) {
        Serial.println(String(i) + " / 7 waiting...");
        delay(1000);
    }


    Serial.println("init of spi!");
    SPI.begin();

// write your initialization code here
    Serial.println("init of imu");
    last_status = imu.begin(true);
    if (last_status == ICM20948::ok) Serial.println("ALL RIGHT");
    else
        Serial.println(last_status);
}

void loop() {
// write your code here

    Serial.println("looopp");

    if (last_status == ICM20948::ok) Serial.println("status: ok");
    else {
        Serial.print("NOT OK: error=");
        Serial.println(last_status);
    }


    delay(1000);
}