//
// Created by dejan on 20/08/2021.
//


#define ICM_WHOAMI 0xEA

#define ICM_REG_WHO_AM_I 0x00

#define ICM_REG_BANK_SELECT 0x7F


#define ICM_REG_PWR_MGMT_1 0x06

typedef struct {
    uint8_t CLKSEL: 3;
    uint8_t TEMP_DIS: 1;
    uint8_t reserved_0: 1;
    uint8_t LP_EN: 1;
    uint8_t SLEEP: 1;
    uint8_t DEVICE_RESET: 1;
} ICM_STRUCT_REG_PWR_MGMT_1_t;
