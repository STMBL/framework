ifneq (,$(findstring units, $(LIBS)))
# lib path
  FIXED_LIBPATH = $(FRAMEWORK_DIR)/libs/units
  $(info using LIB: $(FIXED_LIBPATH))

  INCDIRS := $(INCDIRS) $(FIXED_LIBPATH)/inc
#  LIBSOURCES := $(LIBSOURCES) $(wildcard $(FIXED_LIBPATH)/src/*.cpp)
#  LIBSOURCES := $(LIBSOURCES) $(FIXED_LIBPATH)/src/fp_lib.cpp
 TESTSOURCES := $(TESTSOURCES) $(FIXED_LIBPATH)/test/units.cpp
endif