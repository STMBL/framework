#include "hall_comp.h"
#include "commands.h"
#include "hal.h"
#include "math.h"
#include "defines.h"
#include "angle.h"

HAL_COMP(hall);

HAL_PINA(in, 4);

HAL_PIN(rec);

HAL_PIN(led);

HAL_PINA(p, 16);
HAL_PINA(min, 16);
HAL_PINA(max, 16);

HAL_PIN(pos_in);
HAL_PIN(pos);
HAL_PIN(state);
HAL_PIN(error);

static void rt_func(float period, void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  // struct hall_ctx_t * ctx = (struct hall_ctx_t *)ctx_ptr;
  struct hall_pin_ctx_t *pins = (struct hall_pin_ctx_t *)pin_ptr;

  uint32_t rpos = (PINA(in, 0) > 0.0) * 1 + (PINA(in, 1) > 0.0) * 2 + (PINA(in, 2) > 0.0) * 4 + (PINA(in, 3) > 0.0) * 8;
  PIN(led)      = (PINA(in, 0) > 0.0) ^ (PINA(in, 1) > 0.0) ^ (PINA(in, 2) > 0.0) ^ (PINA(in, 3) > 0.0);

  PIN(state) = 1.0;
  if(PIN(rec) > 0.0){
    if(minus(PIN(pos_in), PINA(min, rpos)) < 0.0){
      PINA(min, rpos) = PIN(pos_in);
    }
    if(minus(PIN(pos_in), PINA(max, rpos)) > 0.0){
      PINA(max, rpos) = PIN(pos_in);
    }
    PINA(p, rpos) = mod(PINA(min, rpos) + minus(PINA(max, rpos), PINA(min, rpos)) / 2.0);
    PIN(state) = 0.0;
  }
  else{
    for(int i = 0; i < 16; i++){
      PINA(min, i) = 10.0;
      PINA(max, i) = -10.0;
    }
  }
  PIN(pos) = PINA(p, rpos);
}

hal_comp_t hall_comp_struct = {
    .name      = "hall",
    .nrt       = 0,
    .rt        = rt_func,
    .frt       = 0,
    .nrt_init  = 0,
    .rt_start  = 0,
    .frt_start = 0,
    .rt_stop   = 0,
    .frt_stop  = 0,
    .ctx_size  = 0,
    .pin_count = sizeof(struct hall_pin_ctx_t) / sizeof(struct hal_pin_inst_t),
};
