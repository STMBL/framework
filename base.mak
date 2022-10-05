# Optimization level, can be [0, 1, 2, 3, s].
#     0 = turn off optimization. s = optimize for size.
#
OPT ?= -O1

# Target file name (without extension)
TARGET = build/fw

FRAMEWORK_DIR = ../../framework

INCDIRS += inc/
# SOURCES += $(wildcard src/*.c)

INCDIRS += build/gen/inc/

CPPFLAGS += -D$(CPUDEF)
CPPFLAGS += -DHSE_VALUE=$(XTAL_FREQ)


#============================================================================
LIBOBJECTS = $(subst ../../framework/libs/,,$(addprefix build/libs/,$(addsuffix .o,$(basename $(LIBSOURCES)))))
GENOBJECTS = $(addsuffix .o,$(basename $(GENSOURCES)))
OBJECTS += $(addprefix build/,$(addsuffix .o,$(basename $(notdir $(SOURCES)))))
OBJECTS += $(LIBOBJECTS) $(GENOBJECTS)
CPPFLAGS += $(addprefix -I,$(INCDIRS))

#---------------- Preprocessor Options ----------------
#  -fsingle...    make better use of the single-precision FPU
#  -g             generate debugging information
#  -save-temps    preserve .s and .i-files
#
CPPFLAGS += -fsingle-precision-constant
# CPPFLAGS += -save-temps=obj

#---------------- C/C++ Compiler Options ----------------
# Use a friendly C dialect
CPPFLAGS += -fno-strict-aliasing
CPPFLAGS += -fwrapv
CPPFLAGS += -ffunction-sections
CPPFLAGS += -fdata-sections
CPPFLAGS += -Wall
CPPFLAGS += -Wmaybe-uninitialized
CPPFLAGS += -Wuninitialized
CPPFLAGS += -Wdouble-promotion
CPPFLAGS += -Wundef
CPPFLAGS += -nodefaultlibs
CPPFLAGS += -nostdlib
CPPFLAGS += -nolibc
CPPFLAGS += -fno-common
CPPFLAGS += -fno-builtin
CPPFLAGS += -nostartfiles
CPPFLAGS += -fstack-usage
CPPFLAGS += -Wpadded
CPPFLAGS += -Wunreachable-code
CPPFLAGS += -Wextra
CPPFLAGS += -Wno-unused-parameter

#---------------- C Compiler Options ----------------
#
CFLAGS += $(OPT)
CFLAGS += -std=gnu11
#CFLAGS += -Wstrict-prototypes
#CFLAGS += -Wpointer-arith
#CFLAGS += -Winline

#---------------- C++ Compiler Options ----------------
#
CXXFLAGS += $(OPT)
CXXFLAGS += -nostdinc++
CXXFLAGS += -std=c++20
CXXFLAGS += -fno-rtti 
CXXFLAGS += -fno-exceptions
CXXFLAGS += -Wno-volatile # allow volatile reg |= bits;
#CXXFLAGS += -lgcc

#---------------- Assembler Options ----------------
#  -Wa,...    tell GCC to pass this to the assembler
#

#---------------- Linker Options ----------------
#  -Wl,...      tell GCC to pass this to linker
#  -Map         create map file
#  --cref       add cross reference to  map file
#
ifndef LDSCRIPT
	LDSCRIPT = cpu.ld
endif

LDFLAGS += $(OPT)
LDFLAGS += -lm
LDFLAGS += -Wl,-Map=build/fw.map,--cref
LDFLAGS += -Wl,--gc-sections
LDFLAGS += -specs=nano.specs -specs=nosys.specs

FINAL_LDFLAGS += $(LDFLAGS)
FINAL_LDFLAGS += -Wl,--print-memory-usage

# LDFLAGS += -specs=nano.specs -u _printf_float -u _scanf_float
# LDFLAGS += -lc -specs=nosys.specs
LDFLAGS += -T$(LDSCRIPT)

#============================================================================

POSTLD   = $(PYTHON) $(FRAMEWORK_DIR)/tools/add_version_info.py # -q

# Compiler flags to generate dependency files
#
GENDEPFLAGS = -MMD -MP

# Combine all necessary flags and optional flags
# Add target processor to flags.
#
ifndef CPUFLAGS
	CPUFLAGS = -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16
endif

CFLAGS   += $(CPUFLAGS)
CXXFLAGS += $(CPUFLAGS)
ASFLAGS  += $(CPUFLAGS)
LDFLAGS  += $(CPUFLAGS)

# Default target
#
all: generate build showsize

build: elf hex bin lss sym

elf: build/fw.elf
hex: build/fw.hex
bin: generate build/fw.bin
lss: build/fw.lss
sym: build/fw.sym

include ../../framework/libs/libs.mak

generate: $(GENINCS) $(GENSOURCES)

# Target: clean project
#
clean:
	@echo Cleaning project:
	rm -rf build

# Include the base rules
#
# Be silent per default, but 'make V=1' will show all compiler calls.
ifneq ($(V),1)
Q     := @
NULL     := 2>/dev/null
endif

GIT_HASH := $(shell git describe --always --dirty)
GIT_BRANCH := $(shell git rev-parse --abbrev-ref HEAD)
USR := $(shell whoami)
HOST := $(shell hostname)
CRC = 0#$(shell crc32 $(basename $@).bin)
BIN_SIZE = $(strip $(shell wc -c < $(basename $@).bin))
CPPFLAGS += -DGIT_HASH=$(GIT_HASH)
CPPFLAGS += -DGIT_BRANCH=$(GIT_BRANCH)
CPPFLAGS += -DUSR=$(USR)
CPPFLAGS += -DHOST=$(HOST)
CPPFLAGS += -DBUILD_CC_VERSION=$(shell $(CC) -dumpversion)
CPPFLAGS += -DBUILD_CC=$(CC)
ifdef XTAL_FREQ
	CFLAGS += -DHSE_VALUE=$(XTAL_FREQ)
endif

# Link: create ELF output file from object files
#

build/fw.tmp_pre2: build/fw.tmp_pre
	@echo Post-processing: $@
	$(Q)$(OBJCOPY) -O binary --gap-fill 0xFF $< $(basename $@).bin

build/fw.elf: build/fw.tmp_pre2
	$(Q)$(CC) $(OBJECTS) $(LDFLAGS) -Xlinker --defsym=BIN_CRC=0 -Xlinker --defsym=BIN_SIZE=$(BIN_SIZE) --output $(basename $@).tmp
	$(Q)$(CC) $(OBJECTS) $(LDFLAGS) -Xlinker --defsym=BIN_CRC=0x$(CRC) -Xlinker --defsym=BIN_SIZE=$(BIN_SIZE) --output $(basename $@).elf

build/fw.tmp_pre: $(OBJECTS) $(LDSCRIPT)
	@echo Linking: $@
	@$(MKDIR) -p $(dir $@)
	$(Q)$(CC) $(OBJECTS) $(FINAL_LDFLAGS) -Xlinker --defsym=BIN_CRC=0 -Xlinker --defsym=BIN_SIZE=0 --output $(basename $@).tmp_pre

# Create extended listing file from ELF output file
#
build/fw.lss: build/fw.elf
	@echo Creating Extended Listing: $@
	@$(MKDIR) -p $(dir $@)
	$(Q)$(OBJDUMP) -h -S -z $< > $@

# Create a symbol table from ELF output file
#
build/fw.sym: build/fw.elf
	@echo Creating Symbol Table: $@
	@$(MKDIR) -p $(dir $@)
	$(Q)$(NM) -n $< > $@

# Create final output files from ELF output file.
#
build/fw.hex: build/fw.elf
	@echo Creating hex file: $@
	@$(MKDIR) -p $(dir $@)
	$(Q)$(OBJCOPY) -O ihex $< $@

# Create binary output file from ELF output file.
#
build/fw.bin: build/fw.elf
	@echo Creating bin file: $@
	@$(MKDIR) -p $(dir $@)
	$(Q)$(OBJCOPY) -O binary --gap-fill 0xFF $< $@

# Compile: create object files from C source files

build/gen/%.o : build/gen/src/%.c $(GENINCS) $(GENSOURCES)
	@echo Compiling gen C: $<
	@$(MKDIR) -p $(dir $@)
	$(Q)$(CC) -c $(CPPFLAGS) $(CFLAGS) $(GENDEPFLAGS) $< -o $@ 

build/libs/%.o : ../../framework/libs/%.c $(GENINCS) $(GENSOURCES)
	@echo Compiling lib C: $<
	@$(MKDIR) -p $(dir $@)
	$(Q)$(CC) -c $(CPPFLAGS) $(CFLAGS) $(GENDEPFLAGS) $< -o $@ 

build/%.o : src/%.c $(GENINCS) $(GENSOURCES)
	@echo Compiling C: $<
	@$(MKDIR) -p $(dir $@)
	$(Q)$(CC) -c $(CPPFLAGS) $(CFLAGS) $(GENDEPFLAGS) $< -o $@ 

# Compile: create object files from C++ source files

build/gen/%.o : build/gen/src/%.cpp $(GENINCS) $(GENSOURCES)
	@echo Compiling gen C++: $<
	@$(MKDIR) -p $(dir $@)
	$(Q)$(Cxx) -c $(CPPFLAGS) $(CXXFLAGS) $(GENDEPFLAGS) $< -o $@ 

build/libs/%.o : ../../framework/libs/%.cpp $(GENINCS) $(GENSOURCES)
	@echo Compiling lib C++: $<
	@$(MKDIR) -p $(dir $@)
	$(Q)$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $(GENDEPFLAGS) $< -o $@ 

build/%.o : src/%.cpp $(GENINCS) $(GENSOURCES)
	@echo Compiling C++: $<
	@$(MKDIR) -p $(dir $@)
	$(Q)$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $(GENDEPFLAGS) $< -o $@ 

# build/%.o : %.cpp
# 	@echo Compiling C++: $<
# 	@$(MKDIR) -p $(dir $@)
# 	$(Q)$(CC) -c $(CPPFLAGS) $(CXXFLAGS) $(GENDEPFLAGS) $< -o $@ 

# Assemble: create object files from assembler source files
#
build/%.o : %.s $(GENINCS) $(GENSOURCES)
	@echo Assembling: $<
	@$(MKDIR) -p $(dir $@)
	$(Q)$(CC) -c $(CPPFLAGS) $(ASFLAGS) $(GENDEPFLAGS) $< -o $@

build/libs/%.o : ../../framework/libs/%.s  $(GENINCS) $(GENSOURCES)
	@echo Assembling: $<
	@$(MKDIR) -p $(dir $@)
	$(Q)$(CC) -c $(CPPFLAGS) $(ASFLAGS) $(GENDEPFLAGS) $< -o $@

%.o: @.d
	$(info no match $@)
# Display compiler version information
#
gccversion:
	@$(CC) --version

# Show the final program size
#
showsize: build
	@echo
	@$(SIZE) build/fw.elf 2>/dev/null

# Flash the device
#
btburn: $(FRAMEWORK_DIR)/tools/bootloader.py build showsize build/fw.bin
	@$(PYTHON) $(FRAMEWORK_DIR)/tools/bootloader.py
	@sleep 1
	@$(DFU-UTIL) -d 0483:df11 -a 0 -s $(ADDRESS):leave -D build/fw.bin

flash: build/fw.bin
	@$(ST-FLASH) --reset write build/fw.bin $(ADDRESS)

# Create a DFU file from bin file
%.dfu: fw.bin
	@cp $< $@
	@$(DFU-SUFFIX) -v 0483 -p df11 -a $@

# TODO: consolidate these two rules into a wildcard version?
build/fw.dfu: $(FRAMEWORK_DIR)/tools/dfu-convert.py build/fw.bin
	$(PYTHON) $(FRAMEWORK_DIR)/tools/dfu-convert.py -b $(ADDRESS):build/fw.bin build/fw.dfu

format: $(SOURCES) $(SRC_COMPS)
	@echo formating code $?
	@clang-format --style=file -i $?

#format:
#	find $(SOURCES) $(INC) -iname '*.h' -o -iname '*.c' | xargs clang-format -i


include ../../framework/toolchain.mak
-include ../../framework/toolchain-user.mak

# Include the dependency files
#
-include $(OBJECTS:.o=.d)

test: $(TESTSOURCES)
	@$(MKDIR) -p build/
	@$(HOSTCXX) -I$(FRAMEWORK_DIR)/libs/catch2/ $(addprefix -I,$(INCDIRS)) -std=c++20 -pedantic -Wall -Wfatal-errors $(TESTSOURCES) $(FRAMEWORK_DIR)/libs/catch2/main.cpp  -o build/test
	build/test

force_look:
	@true

# Listing of phony targets
#
.PHONY: gccversion showsize all build clean \
        elf hex bin lss sym generate \
        format \
        force_look \
				test
