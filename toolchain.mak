# Define programs and commands for the toolchain
# Do not modiy this file. Instead create a file toolchain-user.mak which can overwrite anything in here.
PYTHON     = python3
DFU-UTIL   = dfu-util
DFU-SUFFIX = dfu-suffix
ST-FLASH   = st-flash
J-FLASH    = jlink
TOOLCHAIN  = arm-none-eabi-
CC         = $(TOOLCHAIN)gcc
CXX        = $(TOOLCHAIN)g++
LD         = $(TOOLCHAIN)ld
OBJCOPY    = $(TOOLCHAIN)objcopy
OBJDUMP    = $(TOOLCHAIN)objdump
SIZE       = $(TOOLCHAIN)size
NM         = $(TOOLCHAIN)nm
MKDIR      = mkdir
HOSTCXX    = g++
TIDY		   = clang-tidy