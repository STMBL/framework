ifneq (,$(findstring fixed, $(LIBS)))
# lib path
  FIXED_LIBPATH = $(FRAMEWORK_DIR)/libs/fixed
  $(info using LIB: $(FIXED_LIBPATH))

  INCDIRS := $(INCDIRS) $(FIXED_LIBPATH)/inc
#  LIBSOURCES := $(LIBSOURCES) $(wildcard $(FIXED_LIBPATH)/src/*.cpp)
  LIBSOURCES := $(LIBSOURCES) $(FIXED_LIBPATH)/src/fp_lib.cpp
  TESTSOURCES := $(TESTSOURCES) $(FIXED_LIBPATH)/test/fixed.cpp $(FIXED_LIBPATH)/src/fp_lib.cpp
endif