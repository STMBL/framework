#include <stdint>

class tx_channel{
  public:
    uint8_t* buffer;
    uint32_t buffer_len;
    uint32_t tx_len;
    enum tx_channel_state_t{
      READY,
      BUSY,
      ERROR,
    } state;

  void handler(){
    switch(state){
      case READY:
        if(tx_len > 0){
          // setup iface
          // setup dma
          // start dma
          state = tx_channel_state_t::BUSY;
        }
      break;

      case BUSY:
        // check dma -> error
        // update tx_len
        if(tx_len == 0){
          state = tx_channel_state_t::READY;
        }
      break;

      case ERROR:
      break;
    }
  }
};

class rx_channel{
  public:
    uint8_t* buffer;
    uint32_t buffer_len;
    uint32_t rx_len;
    uint32_t timeout;
    uint32_t timer;
    enum rx_channel_state_t{
      READY,
      ARMED,
      BUSY,
      ERROR,
    } state;

  void handler(){
    switch(state){
      case ARMED:
        rx_len = 0;
        timer = 0;
        // setup dma
        // setup iface
        state = rx_channel_state_t::BUSY;
      break;

      case BUSY:
        // check dma -> error
        if(dma_cnt == rx_len){
          timer++;
          if(timer > timeout){
            state = rx_channel_state_t::READY;
          }
        }
        rx_len = dma_cnt;
      break;

      case ERROR:
      break;
    }
  }
};