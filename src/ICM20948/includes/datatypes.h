//
// Copyright (c) Dejan $originalComment.match("Copyright \(c\) (\d+)", 1, "-")2021.
//

//
// Created by dejan on 22/08/2021.
//

#ifndef ICM20948_DATATYPES_H
#define ICM20948_DATATYPES_H


typedef union {
    int16_t i16bit[3];
    uint8_t u8bit[6];
} ICM_axis3bit16_t;

typedef union {
    ICM_axis3bit16_t raw;
    struct {
        int16_t x, y, z;
    } axis;
} ICM_raw_axis_t;


typedef struct {
    ICM_raw_axis_t acc, gyr, mag;

    union {
        ICM_axis3bit16_t raw;
        int16_t val;
    } tmp;
    //TODO FSS???
    uint8_t magStat1, magStat2;

} ICM_data_raw_t;


typedef struct {
    double x, y, z;
} ICM_axis_t;

typedef struct {
    ICM_axis_t acc, gyr, mag;
    double temp;
    double magAcc;
} ICM_data_converted_t;


#endif //ICM20948_DATATYPES_H
