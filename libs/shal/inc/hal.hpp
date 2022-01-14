
#include <stdint.h>
#include "stm32f1xx.h"
#include <fp_lib.hpp>
#include <sys.hpp>

#pragma once 

class hal_comp{
  public:
    uint32_t min_time;
    uint32_t max_time;
    virtual void rt(hal_ctx_t* ctx, uint32_t hal_slot){}
    virtual void init(hal_ctx_t* ctx){}
    virtual void nrt(hal_ctx_t* ctx){}
};

template<uint32_t slots, uint32_t funcs>
class hal_t{
  public:
    hal_ctx_t* ctx;
    uint64_t hal_ticks;
    uint32_t start_time;
    uint32_t stop_time;

    void run_rt(){
      hal_ticks++;

  
      for(uint32_t i = 0; i < funcs; i++){
        if(comps[hal_slot][i]){
          // GPIOC->ODR |= GPIO_ODR_ODR5;
          start_time = TIM1->CNT;
          if(TIM1->CR1 & TIM_CR1_DIR){
            start_time = TIM1->ARR - start_time;
          }
          else{
            start_time += TIM1->ARR;
          }
          comps[hal_slot][i]->rt(ctx, hal_slot);
          stop_time = TIM1->CNT;

          if(TIM1->CR1 & TIM_CR1_DIR){
            stop_time = TIM1->ARR - stop_time;
          }
          else{
            stop_time += TIM1->ARR;
          }

          comps[hal_slot][i]->min_time = stop_time - start_time;
          comps[hal_slot][i]->max_time = stop_time;
          // GPIOC->ODR &= ~GPIO_ODR_ODR5;
        }
      }

      hal_slot++;
      hal_slot %= slots;
    }

    void run_nrt(){
      for(uint32_t slot = 0; slot < slots; slot++){
        for(uint32_t comp = 0; comp < funcs; comp++){
          if(comps[slot][comp]){
            comps[slot][comp]->nrt(ctx);
          }
        }
      }
    }

    void run_init(){
      hal_slot = 0;
      hal_ticks = 0;

      for(uint32_t slot = 0; slot < slots; slot++){
        for(uint32_t comp = 0; comp < funcs; comp++){
          if(comps[slot][comp]){
            comps[slot][comp]->init(ctx);
          }
        }
      }
    }

    void add_comp(hal_comp* comp, uint32_t slot, uint32_t func){
      if(slot < slots && func < funcs){
        comps[slot][func] = comp;
      }
    }


  private:
    uint32_t hal_slot;
    hal_comp* comps[slots][funcs];
};