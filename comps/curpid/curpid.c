#include "curpid_comp.h"
#include "commands.h"
#include "common.h"
#include "hal.h"
#include "math.h"
#include "defines.h"
#include "angle.h"

HAL_COMP(curpid);

// enable
HAL_PIN(en);
HAL_PIN(cmd_mode); // volt, current
HAL_PIN(mode); // dq transformation

// current command
HAL_PIN(d_cmd);
HAL_PIN(q_cmd);

// current feedback
HAL_PIN(iu);
HAL_PIN(iv);
HAL_PIN(iw);
HAL_PIN(ia);
HAL_PIN(ib);
HAL_PIN(iy);
HAL_PIN(id);
HAL_PIN(iq);

// voltage output
HAL_PIN(ud);
HAL_PIN(uq);
HAL_PIN(ua);
HAL_PIN(ub);
HAL_PIN(uu);
HAL_PIN(uv);
HAL_PIN(uw);

// maximum output current and voltage
HAL_PIN(max_cur);
HAL_PIN(max_volt);

//resistance, d/q inductance, pm flux
HAL_PIN(r);
HAL_PIN(ld);
HAL_PIN(lq);
HAL_PIN(psi);

// gains
HAL_PIN(ff);  // r feed forward
HAL_PIN(kp);
HAL_PIN(ki);
HAL_PIN(ksp); // predictor
HAL_PIN(kind);  // bemf feed forward
HAL_PIN(kci);

// flux gains
HAL_PIN(flp);
HAL_PIN(flux);
HAL_PIN(fkp);
HAL_PIN(ea);
HAL_PIN(eb);
HAL_PIN(fa);
HAL_PIN(fb);
HAL_PIN(min_vel);

HAL_PIN(scale);

// observer
HAL_PIN(pos_in);  // position input
HAL_PIN(pos_out);  // position output
HAL_PIN(vel_out);  // velocity output
// sat, observer stable out, 
HAL_PIN(com_pos);  // commutation position input
HAL_PIN(vel_in);  // velocity input

// current error outputs
HAL_PIN(id_error);
HAL_PIN(iq_error);

HAL_PIN(id_error_sum);
HAL_PIN(iq_error_sum);

HAL_PIN(sin);
HAL_PIN(cos);

HAL_PIN(conv);
HAL_PIN(obs_pos_err);

static void nrt_init(void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  // struct curpid_ctx_t * ctx = (struct curpid_ctx_t *)ctx_ptr;
  struct curpid_pin_ctx_t *pins = (struct curpid_pin_ctx_t *)pin_ptr;

  PIN(r)  = 0.05;
  PIN(ld)  = 0.00006;
  PIN(lq)  = 0.00006;
  PIN(psi) = 0.005;
  PIN(kp)  = 0.5;
  PIN(ki)  = 500.0;
  PIN(kci)  = 500.0;
  PIN(ksp)  = 1.0;
  PIN(scale) = 1.0;

  PIN(flp) = 0.99;
  PIN(fkp) = 1000.0;
}

static void rt_func(float period, void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  // struct curpid_ctx_t *ctx      = (struct curpid_ctx_t *)ctx_ptr;
  struct curpid_pin_ctx_t *pins = (struct curpid_pin_ctx_t *)pin_ptr;

  // clark transformation
  switch((int)PIN(mode)) {
    case PHASE_90_3PH:  // 90°
      PIN(ia) = PIN(iu) - PIN(iv);
      PIN(ib) = PIN(iw) - PIN(iv);
      PIN(iy) = PIN(iu) / 3.0 + PIN(iv) / 3.0 + PIN(iw) / 3.0;
    break;

    case PHASE_120_3PH:  // 120°
      PIN(ia) = PIN(iu) * 2.0 / 3.0 - PIN(iv) / 3.0 - PIN(iw) / 3.0;
      PIN(ib) = PIN(iv) / M_SQRT3 - PIN(iw) / M_SQRT3;
      PIN(iy) = PIN(iu) / 3.0 + PIN(iv) / 3.0 + PIN(iw) / 3.0;
    break;

    case PHASE_180_2PH:  // 180°
      PIN(ia) = 0;
      PIN(ib) = (PIN(iu) - PIN(iw)) / 2.0;
      PIN(iy) = (PIN(iu) + PIN(iw)) / 2.0;
      break;

    case PHASE_180_3PH:  // 180°
      PIN(ia) = PIN(iv);
      PIN(ib) = (PIN(iu) - PIN(iw)) / 2.0;
      PIN(iy) = (PIN(iu) + PIN(iw)) / 2.0;
      break;

    default:
      PIN(ia) = 0.0;
      PIN(ib) = 0.0;
      PIN(iy) = 0.0;
  }

  // flux observer
  PIN(ea) += (PIN(ua) - PIN(ia) * PIN(r)) * period;
  PIN(ea) *= PIN(flp);
  PIN(fa) = PIN(ea) - PIN(ia) * PIN(ld);

  PIN(eb) += (PIN(ub) - PIN(ib) * PIN(r)) * period;
  PIN(eb) *= PIN(flp);
  PIN(fb) = PIN(eb) - PIN(ib) * PIN(ld);

  PIN(flux) = sqrtf(PIN(fa) * PIN(fa) + PIN(fb) * PIN(fb));

  // pos pll
  float pos = PIN(pos_in);
  PIN(conv) = CLAMP(PIN(flux) / (PIN(psi) * 0.9), 0.0, 1.0);
  PIN(conv) *= CLAMP(ABS(PIN(vel_out)) / PIN(min_vel), 0.0, 1.0);

  if(PIN(flux) >= PIN(psi) / 3.0 * 2.0){
  //if(ABS(PIN(vel_out)) > PIN(min_vel)){
    pos = atan2f(PIN(fb), PIN(fa));
  //}
  }

  PIN(obs_pos_err) = minus(pos, PIN(pos_in));

  PIN(pos_out) = mod(PIN(pos_out) + PIN(vel_out) * period);
  float pos_in_error = minus(PIN(pos_in), PIN(pos_out));
  float pos_obs_error = minus(pos, PIN(pos_out));
  float pos_error = PIN(conv) * pos_obs_error + (1.0 - PIN(conv)) * pos_in_error;
  PIN(pos_out) = mod(PIN(pos_out) + pos_error * PIN(fkp) * 2.0 * period);
  PIN(vel_out) += pos_error * PIN(fkp) * PIN(fkp) * period;

  //park transformation
  sincos_fast(PIN(com_pos), &PIN(sin), &PIN(cos));
  PIN(id) = PIN(ia) * PIN(cos) + PIN(ib) * PIN(sin);
  PIN(iq) = -PIN(ia) * PIN(sin) + PIN(ib) * PIN(cos);

  // curpid
  float kpd = PIN(ld) * PIN(kp) / period / 2.0;
  float kid = PIN(r) * PIN(ki);
  float kpq = PIN(lq) * PIN(kp) / period / 2.0;
  float kiq = PIN(r) * PIN(ki);

  float idc = PIN(d_cmd);
  float iqc = PIN(q_cmd);

  float abscur;
  float absvolt;

  if(PIN(cmd_mode) == VOLT_MODE) {
    absvolt = idc * idc + iqc * iqc; // clamp cmd
    PIN(scale) *= sqrtf(CLAMP(PIN(max_volt) * PIN(max_volt) / MAX(absvolt, 0.001), 0.0, 1.0));

    abscur = PIN(id) * PIN(id) + PIN(iq) * PIN(iq); // clamp over fb
    PIN(scale) += (PIN(max_cur) * PIN(max_cur) - abscur) * PIN(kci) * period;
  }
  else{
    abscur = idc * idc + iqc * iqc; // clamp cmd
    PIN(scale) = sqrtf(PIN(max_cur) * PIN(max_cur) / MAX(abscur, 0.001));
  }
  PIN(scale) = CLAMP(PIN(scale), 0.0, 1.0);
  
  idc *= PIN(scale);
  iqc *= PIN(scale);

  float psi_d = PIN(ld) * PIN(id) + PIN(psi);
  float psi_q = PIN(lq) * PIN(iq);
  float indd  = PIN(vel_in) * psi_q;
  float indq  = PIN(vel_in) * psi_d;

  // predictor to cancel pwm delay
  // id += (PIN(ud) - r * id + indd) / ld * period * PIN(ksp);
  // iq += (PIN(uq) - r * iq - indq) / lq * period * PIN(ksp);

  PIN(id_error) = idc - PIN(id);
  PIN(iq_error) = iqc - PIN(iq);

  PIN(ud) = LIMIT(PIN(ff) * PIN(r) * idc - PIN(kind) * indd + kpd * PIN(id_error), PIN(max_volt));
  PIN(uq) = LIMIT(PIN(ff) * PIN(r) * iqc + PIN(kind) * indq + kpq * PIN(iq_error), PIN(max_volt));

  if(PIN(en) > 0.0){
    PIN(id_error_sum) = LIMIT(PIN(id_error_sum) + kid * PIN(id_error) * period, PIN(max_volt) - PIN(ud));
    PIN(iq_error_sum) = LIMIT(PIN(iq_error_sum) + kiq * PIN(iq_error) * period, PIN(max_volt) - PIN(uq));
  }

  PIN(ud) += PIN(id_error_sum);
  PIN(uq) += PIN(iq_error_sum);

  if(PIN(cmd_mode) == VOLT_MODE) {
    PIN(ud)           = idc;
    PIN(uq)           = iqc;
    PIN(id_error_sum) = 0.0;
    PIN(iq_error_sum) = 0.0;
    PIN(id_error)     = 0.0;
    PIN(iq_error)     = 0.0;
  }

  if(PIN(en) <= 0.0) {
    PIN(ud)           = 0.0;
    PIN(uq)           = 0.0;
  }

  if(PIN(en) == 0.0){
    PIN(id_error_sum) = 0.0;
    PIN(iq_error_sum) = 0.0;
  }

  //inverse park transformation
  PIN(ua) = PIN(ud) * PIN(cos) - PIN(uq) * PIN(sin);
  PIN(ub) = PIN(ud) * PIN(sin) + PIN(uq) * PIN(cos);

  //inverse clarke transformation
  switch((int)PIN(mode)) {
    case PHASE_90_3PH:  // 90°
      PIN(uu) = PIN(ua);
      PIN(uv) = 0.0;
      PIN(uw) = PIN(ub);
      break;

    case PHASE_120_3PH:  // 120°
      PIN(uu) = PIN(ua);
      PIN(uv) = -PIN(ua) / 2.0 + PIN(ub) / 2.0 * M_SQRT3;
      PIN(uw) = -PIN(ua) / 2.0 - PIN(ub) / 2.0 * M_SQRT3;
      break;

    case PHASE_180_2PH:  // 180°
      PIN(uu) = PIN(ub) / 2.0;
      PIN(uv) = 0.0;
      PIN(uw) = -PIN(ub) / 2.0;
      break;

    case PHASE_180_3PH:  // 180°
      PIN(uu) = PIN(ub) / 2.0;
      PIN(uv) = PIN(ua);
      PIN(uw) = -PIN(ub) / 2.0;
      break;

    default:
      PIN(uu) = 0.0;
      PIN(uv) = 0.0;
      PIN(uw) = 0.0;
  }
}

hal_comp_t curpid_comp_struct = {
    .name      = "curpid",
    .nrt       = 0,
    .rt        = rt_func,
    .frt       = 0,
    .nrt_init  = nrt_init,
    .rt_start  = 0,
    .frt_start = 0,
    .rt_stop   = 0,
    .frt_stop  = 0,
    .ctx_size  = 0,
    .pin_count = sizeof(struct curpid_pin_ctx_t) / sizeof(struct hal_pin_inst_t),
};
