//
// Copyright (c) Dejan $originalComment.match("Copyright \(c\) (\d+)", 1, "-")2021.
//

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedMacroInspection"

#ifndef ICM_REG_BANK_SELECT
#define ICM_REG_BANK_SELECT 0x7F
#endif

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





// bank2 AGB2_REG_ACCEL_SMPLRT_DIV_1
#define ICM_REG_ACC_SAMPLE_RATE_DIV_1 0x10
#define ICM_REG_ACC_SAMPLE_RATE_DIV_2 0x11



// bank2 AGB2_REG_GYRO_SMPLRT_DIV
#define ICM_REG_GYR_SAMPLE_RATE_DIV 0x00


#pragma clang diagnostic pop