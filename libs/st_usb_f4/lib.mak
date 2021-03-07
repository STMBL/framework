ifneq (,$(findstring STM32F4, $(CPUDEF)))
  ifneq (,$(findstring ST_USB, $(LIBS)))
    # lib path
    LIBPATH = $(FRAMEWORK_DIR)/libs/st_usb_f4
    $(info using LIB: $(LIBPATH))

    # peripheral include
    INCDIRS := $(INCDIRS) $(LIBPATH)/Class/CDC/Inc
    INCDIRS := $(INCDIRS) $(LIBPATH)/Core/Inc
    LIBSOURCES := $(LIBSOURCES) $(LIBPATH)/Class/cdc/src/usbd_cdc.c
    LIBSOURCES := $(LIBSOURCES) $(LIBPATH)/Core/src/usbd_core.c
    LIBSOURCES := $(LIBSOURCES) $(LIBPATH)/Core/src/usbd_ioreq.c
    LIBSOURCES := $(LIBSOURCES) $(LIBPATH)/Core/src/usbd_ctlreq.c
  endif
endif
