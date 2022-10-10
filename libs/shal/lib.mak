ifneq (,$(findstring shal, $(LIBS)))
# lib path
  SHAL_LIBPATH = $(FRAMEWORK_DIR)/libs/shal
  $(info using LIB: $(SHAL_LIBPATH))

  INCDIRS := $(INCDIRS) $(SHAL_LIBPATH)/inc
  LIBSOURCES := $(LIBSOURCES) $(wildcard $(SHAL_LIBPATH)/src/*.cpp)
  GENINCS := build/gen/inc/hal_pins.hpp
endif

build/gen/inc/hal_pins.hpp: $(SRC_COMPS)
	@echo Generating hal pin H: $@
	@$(MKDIR) -p $(dir $@)
	@$(PYTHON) $(SHAL_LIBPATH)/tools/create_hal_pin_h.py $@ $(SRC_COMPS)
