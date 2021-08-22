//
// Created by dejan on 20/08/2021.
//


#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedMacroInspection"
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


#define ICM_INTERNAL_ACC (1 << 0)
#define ICM_INTERNAL_GYR (1 << 1)
#define ICM_INTERNAL_MAG (1 << 2)
#define ICM_INTERNAL_TMP (1 << 3)
#define ICM_INTERNAL_MST (1 <<4 ) // internal i2c maser


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



//ICM_20948_GYRO_CONFIG_1_t - bank 2
#define ICM_REG_GYR_CONF_1 0x01
typedef struct {
    uint8_t GYRO_FCHOICE: 1;
    uint8_t GYRO_FS_SEL: 2;
    uint8_t GYRO_DLPFCFG: 3;
    uint8_t reserved_0: 2;
} ICM_STRUCT_REG_GYR_CONF_1_t;

// FSS CONFIG
#define ICM_CNF_GYR_FSS_DPS250 0x00
#define ICM_CNF_GYR_FSS_DPS500 0x01
#define ICM_CNF_GYR_FSS_DPS1000 0x02
#define ICM_CNF_GYR_FSS_DPS2000 0x03

// DLPF CONFOIG
#define ICM_CNF_GYR_DLPF_D196BW6_N229BW8 0x00
#define ICM_CNF_GYR_DLPF_D151BW8_N187BW6 0x01
#define ICM_CNF_GYR_DLPF_D119BW5_N154BW3 0x02
#define ICM_CNF_GYR_DLPF_D51BW2_N73BW3   0x03
#define ICM_CNF_GYR_DLPF_D23BW9_N35BW9   0x04
#define ICM_CNF_GYR_DLPF_D11BW6_N17BW8   0x05
#define ICM_CNF_GYR_DLPF_D5BW7_N8BW9     0x06
#define ICM_CNF_GYR_DLPF_D361BW4_N376BW5 0x07




#define ICM_REG_ACCEL_XOUT_H 0x2D


//ICM_CNF_ACC_FSS_GMP2 - bank2
#define ICM_REG_ACC_CONF_1 0x14
typedef struct {
    uint8_t ACCEL_FCHOICE: 1;
    uint8_t ACCEL_FS_SEL: 2;
    uint8_t ACCEL_DLPFCFG: 3;
    uint8_t reserved_0: 2;
} ICM_STRUCT_REG_ACC_CONF_1_t;

// Accel FSS config
#define ICM_CNF_ACC_FSS_GMP2 0x00
#define ICM_CNF_ACC_FSS_GMP4 0x01
#define ICM_CNF_ACC_FSS_GMP8 0x02
#define ICM_CNF_ACC_FSS_GMP16 0x03

// Accel DLPF config
#define ICM_CNF_ACC_DLPF_D246BW_N265BW   0x00
#define ICM_CNF_ACC_DLPF_D246BW_N265BW_1 0x01
#define ICM_CNF_ACC_DLPF_D111BW4_N136BW  0x02
#define ICM_CNF_ACC_DLPF_D50BW4_N68BW8   0x03
#define ICM_CNF_ACC_DLPF_D23BW9_N34BW4   0x04
#define ICM_CNF_ACC_DLPF_D11BW5_N17BW    0x05
#define ICM_CNF_ACC_DLPF_D5BW7_B8BW3     0x06
#define ICM_CNF_ACC_DLPF_D473BW_N499BW   0x07


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






// bank2 AGB2_REG_ACCEL_SMPLRT_DIV_1
#define ICM_REG_ACC_SAMPLE_RATE_DIV_1 0x10
#define ICM_REG_ACC_SAMPLE_RATE_DIV_2 0x11



// bank2 AGB2_REG_GYRO_SMPLRT_DIV
#define ICM_REG_GYR_SAMPLE_RATE_DIV 0x00




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



//////////////////// DATATYPES

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

#pragma clang diagnostic pop