#include "fstart_comp.h"
#include "commands.h"
#include "common.h"
#include "hal.h"
#include "math.h"
#include "defines.h"
#include "angle.h"

HAL_COMP(fstart);

HAL_PIN(id_cmd);
HAL_PIN(iq_cmd);

HAL_PIN(pos_in);
HAL_PIN(pos_out);
HAL_PIN(vel);
HAL_PIN(ramp_vel);

HAL_PIN(min_vel);

HAL_PIN(start_cmd);
HAL_PIN(start_ramp);
HAL_PIN(start_acc);
HAL_PIN(start_brake);


static void nrt_init(void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  // struct fstart_ctx_t * ctx = (struct fstart_ctx_t *)ctx_ptr;
  struct fstart_pin_ctx_t *pins = (struct fstart_pin_ctx_t *)pin_ptr;

  PIN(min_vel) = 100;
  // PIN(start_cmd) = 3; // id for forced start
  // PIN(start_ramp) = 0.1; // s to full start_cmd
  PIN(start_acc) = 100; // rad/s^2/A
  // PIN(start_brake) = 0.5; // A
}

static void rt_func(float period, void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  // struct fstart_ctx_t *ctx      = (struct fstart_ctx_t *)ctx_ptr;
  struct fstart_pin_ctx_t *pins = (struct fstart_pin_ctx_t *)pin_ptr;

  if(ABS(PIN(vel)) < PIN(min_vel)){
    PIN(id_cmd) = ABS(PIN(iq_cmd));
//    PIN(id_cmd) = CLAMP(PIN(id_cmd) + PIN(start_cmd) / PIN(start_ramp) * period, 0, PIN(start_cmd));
    PIN(ramp_vel) += PIN(start_acc) * PIN(iq_cmd) * period;
    PIN(ramp_vel) = LIMIT(PIN(ramp_vel), PIN(min_vel));
    // PIN(ramp_vel) -= SIGN(PIN(ramp_vel)) * PIN(start_acc) * PIN(start_brake);
    PIN(ramp_vel) *= 0.9999;
    PIN(pos_out) = mod(PIN(pos_out) + PIN(ramp_vel) * period);
  }
  else{
    PIN(id_cmd) = 0.0;
    // PIN(id_cmd) = CLAMP(PIN(id_cmd) - PIN(start_cmd) / PIN(start_ramp) * period, 0, PIN(start_cmd));
    PIN(pos_out) = PIN(pos_in);
    PIN(ramp_vel) = PIN(vel);
  }
}

hal_comp_t fstart_comp_struct = {
    .name      = "fstart",
    .nrt       = 0,
    .rt        = rt_func,
    .frt       = 0,
    .nrt_init  = nrt_init,
    .rt_start  = 0,
    .frt_start = 0,
    .rt_stop   = 0,
    .frt_stop  = 0,
    .ctx_size  = 0,
    .pin_count = sizeof(struct fstart_pin_ctx_t) / sizeof(struct hal_pin_inst_t),
};
