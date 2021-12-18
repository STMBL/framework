#pragma once

#include <stdint.h>

typedef struct {
  uint32_t crc;
  uint32_t size;
  // void* app;
} bin_info_t;

typedef struct {
  char git_version[32];
  char git_branch[32];
  char build_user[32];
  char build_host[32];
  char build_date[16];
  char build_time[16];

  char product_name[32];
  uint32_t major;
  uint32_t minor;
  uint32_t patch;

  char build_cc[32];
  char cc_version[16];
} fw_info_t;


// typedef struct {

//   fw_info_t* booloader;
//   fw_info_t* app;

//   void* hw_desc;
//   uint32_t hw_desc_size;

//   void* config;
//   uint32_t config_size;

//   void* other_apps;
//   uint32_t other_apps_size;
// } mem_info_t;


// extern volatile const fw_info_t fw_info;
