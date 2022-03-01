
#include <stdint.h>

#pragma once 

template<uint32_t count>
class fifo_t{
  public:
    uint8_t data[count];
    uint32_t read_ptr = 0;
    uint32_t write_ptr = 0;
    uint32_t temp_read_ptr = 0;

  public:
    int push(uint8_t d){
      if((write_ptr + 1) % count != read_ptr){
        data[write_ptr++] = d;
        write_ptr %= count;
        return(1);
      }
      return(0);
    }

    uint8_t pop(){
      uint8_t r = data[read_ptr++];
      read_ptr %= count;
      return(r);
    }

    uint32_t free(){
      return((write_ptr + count - read_ptr) % count);
    }

    uint32_t size(){
      return(count);
    }

    uint8_t* read(){
      return(data + read_ptr);
    }

    uint32_t read_cnt(){
      if(read_ptr <= write_ptr){
        return(write_ptr - read_ptr);
      }
      else{
        return(count - read_ptr);
      }
    }

    void store(uint32_t cnt){
      temp_read_ptr = (read_ptr + cnt) % count;
    }

    void reset(){
      read_ptr = temp_read_ptr;
    }

    uint32_t todo(){
      if(read_ptr == temp_read_ptr){
        if(read_ptr != write_ptr){
          return(1);
        }
      }
      return(0);
    }

    uint8_t* write(){
      return(data + write_ptr);
    }

};
