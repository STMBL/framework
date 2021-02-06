ifneq (,$(findstring STM32F4, $(CPUDEF)))
  ifneq (,$(findstring ST_USB, $(LIBS)))
    # lib path
    LIBPATH = $(FRAMEWORK_DIR)/libs/st_usb_f4
    $(info using LIB: $(LIBPATH))

    # peripheral include
    INCDIRS := $(INCDIRS) $(LIBPATH)/Class/CDC/Inc
    INCDIRS := $(INCDIRS) $(LIBPATH)/Core/Inc
    LIBSOURCES := $(LIBSOURCES) $(LIBPATH)/Class/CDC/Src/usbd_cdc_core.c
    LIBSOURCES := $(LIBSOURCES) $(LIBPATH)/Core/Src/usbd_core.c
    LIBSOURCES := $(LIBSOURCES) $(LIBPATH)/Core/Src/usbd_ioreq.c
    LIBSOURCES := $(LIBSOURCES) $(LIBPATH)/Core/Src/usbd_req.c
  endif
endif
