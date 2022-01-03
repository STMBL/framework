ifneq (,$(findstring fixed, $(LIBS)))
# lib path
  LIBPATH = $(FRAMEWORK_DIR)/libs/fixed
  $(info using LIB: $(LIBPATH))

  INCDIRS := $(INCDIRS) $(LIBPATH)/inc
  LIBSOURCES := $(LIBSOURCES) $(wildcard $(LIBPATH)/src/*.cpp)
endif