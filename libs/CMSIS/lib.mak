ifneq (,$(findstring CMSIS, $(LIBS)))
  # lib path
  LIBPATH = $(FRAMEWORK_DIR)/libs/CMSIS
  $(info using LIB: $(LIBPATH))

  INCDIRS := $(INCDIRS) $(LIBPATH)/Include

  ifneq (,$(findstring STM32F3, $(CPUDEF))) 
    # peripheral include
    INCDIRS := $(INCDIRS) $(LIBPATH)/Device/ST/STM32F3xx/Include
    INCDIRS := $(INCDIRS) $(LIBPATH)/Include

    LIBSOURCES := $(LIBSOURCES) $(LIBPATH)/Device/ST/STM32F3xx/Source/Templates/system_stm32f3xx.c
    LIBSOURCES := $(LIBSOURCES) $(LIBPATH)/Device/ST/STM32F3xx/Source/Templates/gcc/startup_stm32f303xc.s
  endif

  ifneq (,$(findstring STM32F4, $(CPUDEF))) 
    # cpu define
    ifneq (,$(findstring STM32F40, $(CPUDEF)))
      CPUDEF = STM32F40_41xxx
    endif

    # peripheral include
    INCDIRS := $(INCDIRS) $(LIBPATH)/Device/ST/STM32F4xx/Include

    #LIBSOURCES := $(LIBSOURCES) $(LIBPATH)/Device/ST/STM32F4xx/Source/system_stm32f4xx.c
    LIBSOURCES := $(LIBSOURCES) $(LIBPATH)/Device/ST/STM32F4xx/Source/startup_stm32f40_41xxx.s
  endif

  # check cpu define
  ifeq (, $(CPUDEF))
    $(error missing CPU definition)
  endif
endif