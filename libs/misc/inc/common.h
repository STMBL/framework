#pragma once

typedef enum cmd_type_t {
  VOLT_MODE = 0,
  CURRENT_MODE,
} cmd_type;

typedef enum phase_type_t {
  PHASE_90_3PH = 0,
  PHASE_90_4PH,
  PHASE_120_3PH,
  PHASE_180_2PH,
  PHASE_180_3PH,
} phase_type;
  
//fault state
typedef enum {
  DISABLED = 0,
  ENABLED,
  PHASING,
  SOFT_FAULT,
  HARD_FAULT,
  LED_TEST,
} state_t;

//fault error codes
typedef enum {
  NO_ERROR = 0,
  CMD_ERROR,
  MOT_FB_ERROR,
  COM_FB_ERROR,
  JOINT_FB_ERROR,
  POS_ERROR,
  SAT_ERROR,
  MOT_TEMP_ERROR,
  HV_CRC_ERROR,
  HV_TIMEOUT_ERROR,
  HV_TEMP_ERROR,
  HV_VOLT_ERROR,
  HV_FAULT_ERROR,
  HV_CURRENT_OFFSET_FAULT,
  HV_OVERCURRENT_RMS,
  HV_OVERCURRENT_PEAK,
  HV_OVERCURRENT_HW,
} fault_t;
