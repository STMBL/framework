
#include <stdint.h>

#pragma once 

template<uint32_t count>
class fifo_t{
  private:
    uint8_t data[count];
    uint32_t read_ptr = 0; // next index to read
    uint32_t write_ptr = 0; // next index to write
    uint32_t temp_read_ptr = 0; 

  public:
    int push(auto d){
      uint8_t *ptr = &d;
      uint32_t len = sizeof(d);

      uint32_t ret = 0;
      uint32_t cnt = 0;

      do{
        ret = _push(ptr[cnt++]);
      } while(cnt < len && ret == 1);

      return(ret);
    }

    int _push(uint8_t d){
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
      temp_read_ptr = read_ptr;
      return(r);
    }

    void skip(uint32_t i){
      read_ptr += i;
      read_ptr %= count;
      temp_read_ptr = read_ptr;
    }

    uint32_t free(){ // space left
      return(count - (write_ptr + count - read_ptr) % count);
    }

    uint32_t size(){ // all space
      return(count);
    }

    uint32_t available(){
      return(size() - free());
    }

    uint8_t* read(){ // get read ptr
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

    uint32_t todo(){ // stuff to read
      if(read_ptr == temp_read_ptr){
        if(read_ptr != write_ptr){
          return(1);
        }
      }
      return(0);
    }

    uint8_t* write(){ // get write ptr
      return(data + write_ptr);
    }

    void set_write_ptr(uint32_t ptr){
      write_ptr = ptr;
      write_ptr %= count;
    }

    template<uint32_t B>
    uint32_t get(uint32_t i) const {
      union{
        uint32_t ret;
        int8_t bytes[4];
      };
      for(int i = 0; i < B; i++){
        bytes[i] = (*this)[i];
      }
      return(ret);
    }

    uint8_t operator [] (uint32_t i) const {
      return(data[i]);
    }

    uint8_t& operator [] (uint32_t i) {
      return(data[i]);
    }
};
