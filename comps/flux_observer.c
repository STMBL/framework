#include "flux_observer_comp.h"
/*
* This file is part of the stmbl project.
*
* Copyright (C) 2013-2016 Rene Hopf <renehopf@mac.com>
* Copyright (C) 2013-2016 Nico Stute <crinq@crinq.de>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You shou l have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "commands.h"
#include "hal.h"
#include "math.h"
#include "defines.h"
#include "angle.h"


HAL_COMP(flux);

//in
HAL_PIN(r);
HAL_PIN(l);

HAL_PIN(lp);

HAL_PIN(ia);
HAL_PIN(ib);
HAL_PIN(ua);
HAL_PIN(ub);

HAL_PIN(pos_in);
HAL_PIN(kp);

//out
HAL_PIN(vel);
HAL_PIN(pos);
HAL_PIN(poslp);

HAL_PIN(fa);
HAL_PIN(fb);

HAL_PIN(ea);
HAL_PIN(eb);

HAL_PIN(ba);
HAL_PIN(bb);

HAL_PIN(amp);
HAL_PIN(min_amp);

static void nrt_init(void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  // struct flux_ctx_t *ctx      = (struct flux_ctx_t *)ctx_ptr;
  struct flux_pin_ctx_t *pins = (struct flux_pin_ctx_t *)pin_ptr;
  PIN(lp) = 0.99;
  PIN(r) = 0.05;
  PIN(l) = 0.00006;
  PIN(min_amp) = 0.005;
  PIN(kp) = 0.01;
}

static void rt_func(float period, void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  // struct flux_ctx_t *ctx      = (struct flux_ctx_t *)ctx_ptr;
  struct flux_pin_ctx_t *pins = (struct flux_pin_ctx_t *)pin_ptr;

  PIN(ea) += (PIN(ua) - PIN(ia) * PIN(r)) * period;
  PIN(ea) *= PIN(lp);
  PIN(fa) = PIN(ea) - PIN(ia) * PIN(l);
  PIN(ba) = (PIN(ua) - PIN(ia) * PIN(r)) - PIN(ia) * PIN(l);

  PIN(eb) += (PIN(ub) - PIN(ib) * PIN(r)) * period;
  PIN(eb) *= PIN(lp);
  PIN(fb) = PIN(eb) - PIN(ib) * PIN(l);
  PIN(bb) = (PIN(ub) - PIN(ib) * PIN(r)) - PIN(ib) * PIN(l);

  PIN(amp) = sqrtf(PIN(fa) * PIN(fa) + PIN(fb) * PIN(fb));

  if(PIN(amp) >= PIN(min_amp)){
    PIN(pos)  = atan2f(PIN(fb), PIN(fa));
  }

  float pos_error = minus(PIN(pos_in), PIN(pos));
  PIN(pos) = mod(PIN(pos) + pos_error * PIN(kp) * period);
}

hal_comp_t flux_comp_struct = {
    .name      = "flux",
    .nrt       = 0,
    .rt        = rt_func,
    .frt       = 0,
    .nrt_init  = nrt_init,
    .rt_start  = 0,
    .frt_start = 0,
    .rt_stop   = 0,
    .frt_stop  = 0,
    .ctx_size  = 0,
    .pin_count = sizeof(struct flux_pin_ctx_t) / sizeof(struct hal_pin_inst_t),
};