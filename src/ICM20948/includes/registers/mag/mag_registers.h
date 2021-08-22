//
// Copyright (c) Dejan $originalComment.match("Copyright \(c\) (\d+)", 1, "-")2021.
//

/***
 * Magnetometer registers are located here.
 *
 *
 *
 */

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedMacroInspection"


#define ICM_MAG_I2C_ADDR 0x0C

#define ICM_MAG_WHOAMI 0x4809

#define ICM_MAG_REG_WHOAMI1 0
#define ICM_MAG_REG_WHOAMI2 1
#define ICM_MAG_REG_ST1 0x10

//ala AK09916_CNTL2_Reg_t
#define ICM_MAG_REG_CNTL2 0x31
typedef struct {
    uint8_t MODE: 5;
    uint8_t reserved_0: 3;
} ICM_MAG_STRUCT_REG_CNTL2_t;


// aka AK09916_mode_e
#define ICM_MAG_MODE_PWR_DWN 0x00
#define ICM_MAG_MODE_SINGLE (0x01 << 0)
#define ICM_MAG_MODE_CONT_10HZ (0x01 << 1)
#define ICM_MAG_MODE_CONT_20HZ (0x02 << 1)
#define ICM_MAG_MODE_CONT_50HZ (0x03 << 1)
#define ICM_MAG_MODE_CONT_100HZ (0x04 << 1)
#define ICM_MAG_MODE_SELF_TEST (0x01 << 4)


#define ICM_MAG_REG_CNTL3 0x32


#pragma clang diagnostic pop