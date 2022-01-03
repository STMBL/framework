ifneq (,$(findstring shal, $(LIBS)))
# lib path
  LIBPATH = $(FRAMEWORK_DIR)/libs/shal
  $(info using LIB: $(LIBPATH))

  INCDIRS := $(INCDIRS) $(LIBPATH)/inc
  LIBSOURCES := $(LIBSOURCES) $(wildcard $(LIBPATH)/src/*.cpp)
endif