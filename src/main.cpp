#include <Arduino.h>
#include <ICM20948/ICM20948.h>

#include "SPI.h"

#define IMU_CS PA4
#define IMU_INT PA12


ICM20948 imu(IMU_CS, SPI, 7000000, IMU_INT);


ICM20948::status begin_status = ICM20948::unknown;
ICM20948::status set_int_status = ICM20948::unknown;
ICM20948::status conf_int_status = ICM20948::unknown;
ICM20948::status conf_int_latch_status = ICM20948::unknown;
ICM20948::status conf_int_anyre_status = ICM20948::unknown;


unsigned long last_us = 0;

void data_ready() {
    unsigned long ts = micros();

    Serial.println("data ready! " + String(ts - last_us));

    last_us = ts;
}


void checkStatus(ICM20948::status st, String name = "unknown") {

    if (st == ICM20948::ok) Serial.println(name + " status: ok");
    else {
        Serial.print(name + " status is NOT OK: error=");
        Serial.println(st);
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

// write your initialization code here
    Serial.println("init of imu");
    begin_status = imu.begin(true);
    if (begin_status == ICM20948::ok) Serial.println("ALL RIGHT");
    else
        Serial.println(begin_status);


    Serial.println("set autoFetchData");

    set_int_status = imu.autoFetchData(true);
    if (set_int_status == ICM20948::ok) Serial.println("ALL RIGHT autoFetchData");
    else
        Serial.println(set_int_status);

    delay(1000);
}

void loop() {
    if (imu.dataAvailable) {
        Serial.println("new dat");
    }



}