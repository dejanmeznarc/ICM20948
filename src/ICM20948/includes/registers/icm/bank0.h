//
// Copyright (c) Dejan $originalComment.match("Copyright \(c\) (\d+)", 1, "-")2021.
//

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedMacroInspection"

#ifndef ICM_REG_BANK_SELECT
#define ICM_REG_BANK_SELECT 0x7F
#endif

#define ICM_WHOAMI 0xEA
#define ICM_REG_WHO_AM_I 0x00


#define ICM_REG_ACCEL_XOUT_H 0x2D



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



// aka AGB0_REG_LP_CONFIG, ICM_20948_LP_CONFIG_t
#define ICM_REG_LP_CONFIG 0x05
typedef struct {
    uint8_t reserved_0: 4;
    uint8_t GYRO_CYCLE: 1;
    uint8_t ACCEL_CYCLE: 1;
    uint8_t I2C_MST_CYCLE: 1;
    uint8_t reserved_1: 1;
} ICM_STRUCT_REG_LP_CONFIG_t;


#define ICM_CNF_SAMPLE_MODE_CONT 0x00
#define ICM_CNF_SAMPLE_MODE_CYCLE 0x01

#define ICM_INTERNAL_ACC (1 << 0)
#define ICM_INTERNAL_GYR (1 << 1)
#define ICM_INTERNAL_MAG (1 << 2)
#define ICM_INTERNAL_TMP (1 << 3)
#define ICM_INTERNAL_MST (1 <<4 ) // internal i2c maser


typedef struct {
    uint8_t I2C_MST_INT_EN: 1;
    uint8_t DMP_INT1_EN: 1;
    uint8_t PLL_READY_EN: 1;
    uint8_t WOM_INT_EN: 1;
    uint8_t reserved_0: 3;
    uint8_t REG_WOF_EN: 1;
} ICM_20948_INT_ENABLE_t;


// ICM_20948_INT_ENABLE_1_t bank 0
#define ICM_REG_INT_ENABLE_1 0x11
typedef struct {
    uint8_t RAW_DATA_0_RDY_EN: 1;
    uint8_t reserved_0: 7;
} ICM_STRUCT_REG_INT_ENABLE_1_t;


typedef union {
    struct {
        uint8_t FIFO_OVERFLOW_EN_40: 5;
        uint8_t reserved_0: 3;
    } grouped;
    struct {
        uint8_t FIFO_OVERFLOW_EN_0: 1;
        uint8_t FIFO_OVERFLOW_EN_1: 1;
        uint8_t FIFO_OVERFLOW_EN_2: 1;
        uint8_t FIFO_OVERFLOW_EN_3: 1;
        uint8_t FIFO_OVERFLOW_EN_4: 1;
        uint8_t reserved_0: 3;
    } individual;
} ICM_20948_INT_ENABLE_2_t;

typedef union {
    struct {
        uint8_t FIFO_WM_EN_40: 5;
        uint8_t reserved_0: 3;
    } grouped;
    struct {
        uint8_t FIFO_WM_EN_0: 1;
        uint8_t FIFO_WM_EN_1: 1;
        uint8_t FIFO_WM_EN_2: 1;
        uint8_t FIFO_WM_EN_3: 1;
        uint8_t FIFO_WM_EN_4: 1;
        uint8_t reserved_0: 3;
    } individual;
} ICM_20948_INT_ENABLE_3_t;



// aka. AGB0_REG_INT_PIN_CONFIG & ICM_20948_INT_PIN_CFG_t
#define ICM_REG_INT_PIN_CONF 0x15
typedef struct {
    uint8_t reserved_0: 1;
    uint8_t BYPASS_EN: 1;
    uint8_t FSYNC_INT_MODE_EN: 1;
    uint8_t ACTL_FSYNC: 1;
    uint8_t INT_ANYRD_2CLEAR: 1;
    uint8_t INT1_LATCH_EN: 1;
    uint8_t INT1_OPEN: 1;
    uint8_t INT1_ACTL: 1;
} ICM_STRUCT_REG_INT_PIN_CONF_t;


#pragma clang diagnostic pop