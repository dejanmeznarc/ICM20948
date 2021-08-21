//
// Created by dejan on 20/08/2021.
//


#define ICM_WHOAMI 0xEA

#define ICM_REG_WHO_AM_I 0x00

#define ICM_REG_BANK_SELECT 0x7F

// bank 0
#define ICM_REG_PWR_MGMT_1 0x06
typedef struct {
    uint8_t CLKSEL: 3;
    uint8_t TEMP_DIS: 1;
    uint8_t reserved_0: 1;
    uint8_t LP_EN: 1;
    uint8_t SLEEP: 1;
    uint8_t DEVICE_RESET: 1;
} ICM_STRUCT_REG_PWR_MGMT_1_t;


// bank 0
#define ICM_REG_INT_PIN_CNFG 0x0F
typedef struct {
    uint8_t reserved_0: 1;
    uint8_t BYPASS_EN: 1;
    uint8_t FSYNC_INT_MODE_EN: 1;
    uint8_t ACTL_FSYNC: 1;
    uint8_t INT_ANYRD_2CLEAR: 1;
    uint8_t INT1_LATCH_EN: 1;
    uint8_t INT1_OPEN: 1;
    uint8_t INT1_ACTL: 1;
} ICM_STRUCT_REG_INT_PIN_CNFG_t;


//ICM_20948_I2C_MST_CTRL_t - bank 3
#define ICM_REG_I2C_MST_CTRL 0x01
typedef struct {
    uint8_t I2C_MST_CLK: 4;
    uint8_t I2C_MST_P_NSR: 1;
    uint8_t reserved_0: 2;
    uint8_t MULT_MST_EN: 1;
} ICM_STRUCT_REG_I2C_MST_CTRL_t;


//ICM_20948_USER_CTRL_t - bank 0
#define ICM_REG_USER_CTRL 0x03
typedef struct {
    uint8_t reserved_0: 1;
    uint8_t I2C_MST_RST: 1;
    uint8_t SRAM_RST: 1;
    uint8_t DMP_RST: 1;
    uint8_t I2C_IF_DIS: 1;
    uint8_t I2C_MST_EN: 1;
    uint8_t FIFO_EN: 1;
    uint8_t DMP_EN: 1;
} ICM_STRUCT_REG_USER_CTRL_t;








// aka AGB3_REG_I2C_PERIPH4_ADDR
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









//////////////// MAGNETOMETER MAP
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


