#include <Arduino.h>
#include <ICM20948/ICM20948.h>

#include "SPI.h"

#define IMU_CS PA4
#define IMU_INT PA12


ICM20948 imu(IMU_CS, SPI, 7000000);

void setup() {
    Serial.begin(1000000);
    Serial.println("Hello world!");

    for (int i = 0; i <= 5; ++i) {
        Serial.println(String(i) + " / 5 waiting...");
        delay(1000);
    }



    Serial.println("init of spi!");
    SPI.begin();

// write your initialization code here
    Serial.println("init of imu");
    ICM20948::status status = imu.begin();
    if (status == ICM20948::ok) Serial.println("ALL RIGHT");
    else
        Serial.println(status);
}

void loop() {
// write your code here

    Serial.println("looopp");
    delay(1000);
}