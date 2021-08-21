#include <Arduino.h>
#include <ICM20948/ICM20948.h>

#include "SPI.h"

#define IMU_CS PA4
#define IMU_INT PA12


ICM20948 imu(IMU_CS, SPI, 7000000);


ICM20948::status begin_status = ICM20948::unknown;
ICM20948::status set_int_status = ICM20948::unknown;


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
    begin_status = imu.begin(true);
    if (begin_status == ICM20948::ok) Serial.println("ALL RIGHT");
    else
        Serial.println(begin_status);


    Serial.println("set interrupt");

    set_int_status = imu.setIntEnableOnRawDataReady(true);
    if (begin_status == ICM20948::ok) Serial.println("ALL RIGHT 2");
    else
        Serial.println(begin_status);

}

void loop() {
// write your code here

    Serial.println("\n \n");

    if (begin_status == ICM20948::ok) Serial.println("begin status: ok");
    else {
        Serial.print("begin status is NOT OK: error=");
        Serial.println(begin_status);
    }


    if (begin_status == ICM20948::ok) Serial.println("int status: ok");
    else {
        Serial.print("int status is NOT OK: error=");
        Serial.println(begin_status);
    }


    delay(1000);
}