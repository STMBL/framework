ifneq (,$(findstring units, $(LIBS)))
# lib path
  UNITS_LIBPATH = $(FRAMEWORK_DIR)/libs/units
  $(info using LIB: $(UNITS_LIBPATH))

  INCDIRS := $(INCDIRS) $(UNITS_LIBPATH)/inc
#  LIBSOURCES := $(LIBSOURCES) $(wildcard $(UNITS_LIBPATH)/src/*.cpp)
#  LIBSOURCES := $(LIBSOURCES) $(UNITS_LIBPATH)/src/fp_lib.cpp
 TESTSOURCES := $(TESTSOURCES) $(UNITS_LIBPATH)/test/units.cpp
endif