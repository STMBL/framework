ifneq (,$(findstring cpp_conf, $(LIBS)))
# lib path
  LIBPATH = $(FRAMEWORK_DIR)/libs/cpp_conf
  $(info using LIB: $(LIBPATH))

  INCDIRS := $(INCDIRS) $(LIBPATH)/inc
  LIBSOURCES := $(LIBSOURCES) $(wildcard $(LIBPATH)/src/*.cpp)
  GENINCS += build/gen/inc/gen_conf.hpp
  GENSOURCES += build/gen/src/gen_conf.cpp
endif

build/gen/src/gen_conf.cpp: $(FRAMEWORK_DIR)/tools/create_cmd.py inc/conf.inc
	@echo Generating cpp conf
	@$(MKDIR) -p $(dir build/gen/src/gen_conf.cpp)
	@$(MKDIR) -p $(dir build/gen/inc/gen_conf.hpp)
	@$(PYTHON) $(FRAMEWORK_DIR)/tools/gen_cpp_conf.py inc/conf.inc build/gen/inc/gen_conf.hpp build/gen/src/gen_conf.cpp

build/gen/inc/gen_conf.hpp: $(FRAMEWORK_DIR)/tools/create_cmd.py inc/conf.inc
	@echo Generating cpp conf
	@$(MKDIR) -p $(dir build/gen/src/gen_conf.cpp)
	@$(MKDIR) -p $(dir build/gen/inc/gen_conf.hpp)
	@$(PYTHON) $(FRAMEWORK_DIR)/tools/gen_cpp_conf.py inc/conf.inc build/gen/inc/gen_conf.hpp build/gen/src/gen_conf.cpp