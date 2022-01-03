#include <stdint.h>
#include <fp_lib.hpp>
#include <sys.hpp>
#pragma once 

class hal_comp{
  public:
    virtual void rt_func(hal_ctx_t* ctx, uint32_t hal_slot){}
    virtual void init(hal_ctx_t* ctx){}
};

template<uint32_t slots, uint32_t funcs>
class hal_t{
  public:
    hal_ctx_t* ctx;
    uint64_t hal_ticks;

    void run_rt(){
      hal_ticks++;
  
      for(uint32_t i = 0; i < funcs; i++){
        if(comps[hal_slot][i]){
          comps[hal_slot][i]->rt_func(ctx, hal_slot);
        }
      }

      hal_slot++;
      hal_slot %= slots;
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