ifneq (,$(findstring STM32F1, $(CPUDEF)))
  ifneq (,$(findstring ST_HAL, $(LIBS)))
    # lib path
    LIBPATH = $(FRAMEWORK_DIR)/libs/st_hal_f1
    $(info using LIB: $(LIBPATH))

    # peripheral include
    INCDIRS := $(INCDIRS) $(LIBPATH)/inc
    LIBSOURCES := $(LIBSOURCES) $(LIBPATH)/src/stm32f1xx_hal.c
    LIBSOURCES := $(LIBSOURCES) $(LIBPATH)/src/stm32f1xx_hal_cortex.c

    ifneq (,$(findstring UART, $(PERIPH)))
      LIBSOURCES := $(LIBSOURCES) $(LIBPATH)/src/stm32f1xx_hal_uart.c
      LIBSOURCES := $(LIBSOURCES) $(LIBPATH)/src/stm32f1xx_hal_uart_ex.c
    endif

    ifneq (,$(findstring COMP, $(PERIPH)))
      LIBSOURCES := $(LIBSOURCES) $(LIBPATH)/src/stm32f1xx_hal_comp.c
    endif

    ifneq (,$(findstring ADC, $(PERIPH)))
      LIBSOURCES := $(LIBSOURCES) $(LIBPATH)/src/stm32f1xx_hal_adc.c
      LIBSOURCES := $(LIBSOURCES) $(LIBPATH)/src/stm32f1xx_hal_adc_ex.c
    endif

    ifneq (,$(findstring TIM, $(PERIPH)))
      LIBSOURCES := $(LIBSOURCES) $(LIBPATH)/src/stm32f1xx_hal_tim.c
      LIBSOURCES := $(LIBSOURCES) $(LIBPATH)/src/stm32f1xx_hal_tim_ex.c
    endif

    ifneq (,$(findstring DAC, $(PERIPH)))
      LIBSOURCES := $(LIBSOURCES) $(LIBPATH)/src/stm32f1xx_hal_dac.c
      LIBSOURCES := $(LIBSOURCES) $(LIBPATH)/src/stm32f1xx_hal_dac_ex.c
    endif

    ifneq (,$(findstring GPIO, $(PERIPH)))
      LIBSOURCES := $(LIBSOURCES) $(LIBPATH)/src/stm32f1xx_hal_gpio.c
    endif

    ifneq (,$(findstring CRC, $(PERIPH)))
      LIBSOURCES := $(LIBSOURCES) $(LIBPATH)/src/stm32f1xx_hal_crc.c
      LIBSOURCES := $(LIBSOURCES) $(LIBPATH)/src/stm32f1xx_hal_crc_ex.c
    endif

    ifneq (,$(findstring RCC, $(PERIPH)))
      LIBSOURCES := $(LIBSOURCES) $(LIBPATH)/src/stm32f1xx_hal_rcc.c
      LIBSOURCES := $(LIBSOURCES) $(LIBPATH)/src/stm32f1xx_hal_rcc_ex.c
    endif

    ifneq (,$(findstring PWR, $(PERIPH)))
      LIBSOURCES := $(LIBSOURCES) $(LIBPATH)/src/stm32f1xx_hal_pwr.c
      LIBSOURCES := $(LIBSOURCES) $(LIBPATH)/src/stm32f1xx_hal_pwr_ex.c
    endif

    ifneq (,$(findstring FLASH, $(PERIPH)))
      LIBSOURCES := $(LIBSOURCES) $(LIBPATH)/src/stm32f1xx_hal_flash.c
      LIBSOURCES := $(LIBSOURCES) $(LIBPATH)/src/stm32f1xx_hal_flash_ex.c
    endif

    ifneq (,$(findstring DMA, $(PERIPH)))
      LIBSOURCES := $(LIBSOURCES) $(LIBPATH)/src/stm32f1xx_hal_dma.c
    endif

    ifneq (,$(findstring USB, $(PERIPH)))
      LIBSOURCES := $(LIBSOURCES) $(LIBPATH)/src/stm32f1xx_hal_pcd.c
      LIBSOURCES := $(LIBSOURCES) $(LIBPATH)/src/stm32f1xx_hal_pcd_ex.c
      LIBSOURCES := $(LIBSOURCES) $(LIBPATH)/src/stm32f1xx_ll_usb.c
    endif

    ifneq (,$(findstring SPI, $(PERIPH)))
      LIBSOURCES := $(LIBSOURCES) $(LIBPATH)/src/stm32f1xx_hal_spi.c
      LIBSOURCES := $(LIBSOURCES) $(LIBPATH)/src/stm32f1xx_hal_spi_ex.c
    endif
  endif
endif
