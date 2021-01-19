#include "hfi_comp.h"
#include "commands.h"
#include "common.h"
#include "hal.h"
#include "math.h"
#include "defines.h"
#include "angle.h"

HAL_COMP(hfi);

HAL_PIN(id_cmd);
HAL_PIN(iq_fb);

HAL_PIN(pos);

HAL_PIN(id_offset);
HAL_PIN(id_amp);
HAL_PIN(state);

HAL_PIN(err);
HAL_PIN(sum);
HAL_PIN(ki);
HAL_PIN(lpf);

HAL_PIN(time);
HAL_PIN(timer);

static void nrt_init(void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  // struct hfi_ctx_t * ctx = (struct hfi_ctx_t *)ctx_ptr;
  struct hfi_pin_ctx_t *pins = (struct hfi_pin_ctx_t *)pin_ptr;

  PIN(ki)  = -0.1;
  PIN(lpf)  = 0.9995;
  PIN(id_offset) = 4;
  PIN(id_amp)  = 1;
  PIN(state)  = -1.0;
  PIN(timer)  = 0.0;
  PIN(time)  = 0.05;

}

static void rt_func(float period, void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  // struct hfi_ctx_t *ctx      = (struct hfi_ctx_t *)ctx_ptr;
  struct hfi_pin_ctx_t *pins = (struct hfi_pin_ctx_t *)pin_ptr;

  if(PIN(timer) >= PIN(time)){
    PIN(state) *= -1;
    PIN(timer) = 0.0;
  }
  PIN(timer) += period;


  PIN(id_cmd) = PIN(id_offset) + PIN(state) * PIN(id_amp);
  PIN(err) = PIN(state) * PIN(id_amp) * PIN(iq_fb);
  PIN(sum) += PIN(err) * period;
  PIN(sum) *= PIN(lpf);
  PIN(pos) = mod(PIN(pos) + PIN(ki) * PIN(err));

  PIN(state) = -PIN(state);
}

hal_comp_t hfi_comp_struct = {
    .name      = "hfi",
    .nrt       = 0,
    .rt        = rt_func,
    .frt       = 0,
    .nrt_init  = nrt_init,
    .rt_start  = 0,
    .frt_start = 0,
    .rt_stop   = 0,
    .frt_stop  = 0,
    .ctx_size  = 0,
    .pin_count = sizeof(struct hfi_pin_ctx_t) / sizeof(struct hal_pin_inst_t),
};
