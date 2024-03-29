ifneq (,$(findstring CMSIS, $(LIBS)))
  # lib path
  CMSIS_LIBPATH = $(FRAMEWORK_DIR)/libs/CMSIS
  $(info using LIB: $(CMSIS_LIBPATH))

  INCDIRS := $(INCDIRS) $(CMSIS_LIBPATH)/Include

  ifneq (,$(findstring STM32F1, $(CPUDEF)))
    # peripheral include
    INCDIRS := $(INCDIRS) $(CMSIS_LIBPATH)/Device/ST/STM32F1xx/Include
    INCDIRS := $(INCDIRS) $(CMSIS_LIBPATH)/Include

    LIBSOURCES := $(LIBSOURCES) $(CMSIS_LIBPATH)/Device/ST/STM32F1xx/Source/Templates/system_stm32f1xx.c
    LIBSOURCES := $(LIBSOURCES) $(CMSIS_LIBPATH)/Device/ST/STM32F1xx/Source/Templates/gcc/startup_stm32f103xb.s
  endif

  ifneq (,$(findstring STM32F3, $(CPUDEF))) 
    # peripheral include
    INCDIRS := $(INCDIRS) $(CMSIS_LIBPATH)/Device/ST/STM32F3xx/Include
    INCDIRS := $(INCDIRS) $(CMSIS_LIBPATH)/Include

    LIBSOURCES := $(LIBSOURCES) $(CMSIS_LIBPATH)/Device/ST/STM32F3xx/Source/Templates/system_stm32f3xx.c
    LIBSOURCES := $(LIBSOURCES) $(CMSIS_LIBPATH)/Device/ST/STM32F3xx/Source/Templates/gcc/startup_stm32f303xc.s
  endif

  ifneq (,$(findstring STM32F405, $(CPUDEF))) 
    # # cpu define
    # ifneq (,$(findstring STM32F40, $(CPUDEF)))
    #   CPUDEF = STM32F405xx
    # endif

    # peripheral include
    INCDIRS := $(INCDIRS) $(CMSIS_LIBPATH)/Device/ST/STM32F4xx/Include
    INCDIRS := $(INCDIRS) $(CMSIS_LIBPATH)/Include

    LIBSOURCES := $(LIBSOURCES) $(CMSIS_LIBPATH)/Device/ST/STM32F4xx/Source/Templates/system_stm32f4xx.c
    LIBSOURCES := $(LIBSOURCES) $(CMSIS_LIBPATH)/Device/ST/STM32F4xx/Source/Templates/gcc/startup_stm32f405xx.s
  endif

  # check cpu define
  ifeq (, $(CPUDEF))
    $(error missing CPU definition)
  endif
endif