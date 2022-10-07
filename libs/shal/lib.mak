ifneq (,$(findstring shal, $(LIBS)))
# lib path
  LIBPATH = $(FRAMEWORK_DIR)/libs/shal
  $(info using LIB: $(LIBPATH))

  INCDIRS := $(INCDIRS) $(LIBPATH)/inc
  LIBSOURCES := $(LIBSOURCES) $(wildcard $(LIBPATH)/src/*.cpp)
  GENINCS := build/gen/inc/hal_pins.hpp
endif

build/gen/inc/hal_pins.hpp: $(SRC_COMPS)
	@echo Generating hal pin H: $@
	@$(MKDIR) -p $(dir $@)
	@$(PYTHON) $(LIBPATH)/tools/create_hal_pin_h.py $@ $(SRC_COMPS)
