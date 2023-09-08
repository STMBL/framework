
#include <stdint.h>
#include <fp_lib.hpp>
#include "hal_pins.hpp"

#pragma once 

class hal_comp{
  public:
    uint32_t time = 0;
    uint32_t max_time = 0;
    virtual void rt(hal_ctx_t* ctx, uint32_t hal_slot){}
    virtual void init(hal_ctx_t* ctx){}
    virtual void nrt(hal_ctx_t* ctx){}
};

template<uint32_t slots, uint32_t funcs>
class hal_t{
  public:
    uint64_t hal_ticks;
    hal_ctx_t* ctx;

    volatile uint32_t * tim_ptr;

    void run_rt(){
      hal_ticks++;
  
      for(uint32_t i = 0; i < funcs; i++){
        if(rt_comps[hal_slot][i]){
          int32_t start_time = *tim_ptr;
          rt_comps[hal_slot][i]->rt(ctx, hal_slot);
          int32_t stop_time = *tim_ptr;
          int32_t diff = stop_time - start_time;
          diff = ABS(diff);
          rt_comps[hal_slot][i]->time = diff;
          rt_comps[hal_slot][i]->max_time = MAX(rt_comps[hal_slot][i]->max_time, diff);
        }
      }

      hal_slot++;
      hal_slot %= slots;
    }

    void run_nrt(){
      for(uint32_t comp = 0; comp < comp_cnt; comp++){
        comps[comp]->nrt(ctx);
      }
    }

    void run_init(){
      hal_slot = 0;
      hal_ticks = 0;

      for(uint32_t comp = 0; comp < comp_cnt; comp++){
        comps[comp]->time = 0;
        comps[comp]->max_time = 0;
        comps[comp]->init(ctx);
      }
    }

    void add_rt_comp(hal_comp* comp, uint32_t slot, uint32_t func){
      if(slot < slots && func < funcs){
        rt_comps[slot][func] = comp;
      }
    }

    void add_comp(hal_comp* comp){
      if(comp_cnt < slots * funcs){
        comps[comp_cnt] = comp;
        comp_cnt++;
      }
    }


    hal_comp* rt_comps[slots][funcs];
    hal_comp* comps[slots * funcs];
  private:
    uint32_t comp_cnt = 0;
    uint32_t hal_slot;
};