//
// Copyright (c) Dejan $originalComment.match("Copyright \(c\) (\d+)", 1, "-")2021.
//

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedMacroInspection"

#ifndef ICM_REG_BANK_SELECT
#define ICM_REG_BANK_SELECT 0x7F
#endif

//ICM_20948_I2C_MST_CTRL_t - bank 3
#define ICM_REG_I2C_MST_CTRL 0x01
typedef struct {
    uint8_t I2C_MST_CLK: 4;
    uint8_t I2C_MST_P_NSR: 1;
    uint8_t reserved_0: 2;
    uint8_t MULT_MST_EN: 1;
} ICM_STRUCT_REG_I2C_MST_CTRL_t;



// aka AGB3_REG_I2C_PERIPH4_ADDR bank 3
#define ICM_REG_I2C_SLV0_ADDR 0x03
#define ICM_REG_I2C_SLV1_ADDR 0x07
#define ICM_REG_I2C_SLV2_ADDR 0x0B
#define ICM_REG_I2C_SLV3_ADDR 0x0F
#define ICM_REG_I2C_SLV4_ADDR 0x13
//ICM_20948_I2C_PERIPHX_ADDR_t
typedef struct {
    uint8_t ID: 7;
    uint8_t RNW: 1;
} ICM_STRUCT_I2C_SLV_ADDR_t;

// aka AGB3_REG_I2C_PERIPH4_REG
#define ICM_REG_I2C_SLV0_REG 0x04
#define ICM_REG_I2C_SLV1_REG 0x08
#define ICM_REG_I2C_SLV2_REG 0x0C
#define ICM_REG_I2C_SLV3_REG 0x10
#define ICM_REG_I2C_SLV4_REG 0x14
//ICM_20948_I2C_PERIPHX_REG_t
typedef struct {
    uint8_t REG;
} ICM_STRUCT_REG_I2C_SLV_REG_t;




//ICM_20948_I2C_PERIPH4_CTRL_t
// aka AGB3_REG_I2C_PERIPH4_CTRL
#define ICM_REG_I2C_SLV0_CTRL 0x05
#define ICM_REG_I2C_SLV1_CTRL 0x09
#define ICM_REG_I2C_SLV2_CTRL 0x0D
#define ICM_REG_I2C_SLV3_CTRL 0x11
//ICM_20948_I2C_PERIPHX_CTRL_t
typedef struct {
    uint8_t LENG: 4;
    uint8_t GRP: 1;
    uint8_t REG_DIS: 1;
    uint8_t BYTE_SW: 1;
    uint8_t EN: 1;
} ICM_STRUCT_REG_I2C_SLV_CTRL_t;


#define ICM_REG_I2C_SLV4_CTRL 0x15
typedef struct {
    uint8_t DLY: 5;
    uint8_t REG_DIS: 1;
    uint8_t INT_EN: 1;
    uint8_t EN: 1;
} ICM_STRUCT_REG_I2C_SLV4_CTRL_t;


// aka AGB0_REG_I2C_MST_STATUS 23
#define ICM_REG_I2C_SLV4_DI 0x17
typedef struct {
    uint8_t I2C_PERIPH0_NACK: 1;
    uint8_t I2C_PERIPH1_NACK: 1;
    uint8_t I2C_PERIPH2_NACK: 1;
    uint8_t I2C_PERIPH3_NACK: 1;
    uint8_t I2C_PERIPH4_NACK: 1;
    uint8_t I2C_LOST_ARB: 1;
    uint8_t I2C_PERIPH4_DONE: 1;
    uint8_t PASS_THROUGH: 1;
} ICM_STRUCT_REG_I2C_SLV4_DI_t;


#define ICM_REG_I2C_SLV0_DO 0x06
#define ICM_REG_I2C_SLV1_DO 0x0A
#define ICM_REG_I2C_SLV2_DO 0x0E
#define ICM_REG_I2C_SLV3_DO 0x12
#define ICM_REG_I2C_SLV4_DO 0x16

//ICM_20948_I2C_PERIPHX_DO_t
typedef struct {
    uint8_t DO;
} ICM_STRUCT_REG_I2C_SLV_DO_t;


#pragma clang diagnostic pop