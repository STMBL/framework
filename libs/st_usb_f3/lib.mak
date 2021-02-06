ifneq (,$(findstring STM32F3, $(CPUDEF)))
  ifneq (,$(findstring ST_USB, $(LIBS)))
    # lib path
    LIBPATH = $(FRAMEWORK_DIR)/libs/st_usb_f3
    $(info using LIB: $(LIBPATH))
    INCDIRS := $(INCDIRS) $(LIBPATH)/Core/Inc/
    INCDIRS := $(INCDIRS) $(LIBPATH)/Class/CDC/Inc/
    # INCDIRS := $(INCDIRS) $(LIBPATH)/Class/DFU/Inc/

    LIBSOURCES := $(LIBSOURCES) $(LIBPATH)/Core/Src/usbd_core.c
    LIBSOURCES := $(LIBSOURCES) $(LIBPATH)/Core/Src/usbd_ctlreq.c
    LIBSOURCES := $(LIBSOURCES) $(LIBPATH)/Core/Src/usbd_ioreq.c

    LIBSOURCES := $(LIBSOURCES) $(LIBPATH)/Class/CDC/Src/usbd_cdc.c
    # LIBSOURCES := $(LIBSOURCES) $(LIBPATH)/Class/DFU/Src/usbd_dfu.c
  endif
endif
