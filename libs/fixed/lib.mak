ifneq (,$(findstring fixed, $(LIBS)))
# lib path
  LIBPATH = $(FRAMEWORK_DIR)/libs/fixed
  $(info using LIB: $(LIBPATH))

  INCDIRS := $(INCDIRS) $(LIBPATH)/inc
#  LIBSOURCES := $(LIBSOURCES) $(wildcard $(LIBPATH)/src/*.cpp)
  LIBSOURCES := $(LIBSOURCES) $(LIBPATH)/src/fp_lib.cpp
  TESTSOURCES := $(TESTSOURCES) $(LIBPATH)/test/fixed.cpp $(LIBPATH)/src/fp_lib.cpp
endif