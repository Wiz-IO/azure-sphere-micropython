CROSS_DIR = C:/Users/HP/.platformio/packages/toolchain-arm-poky-linux-musleabi-hf
SYSROOT = C:/Users/HP/.platformio/packages/framework-azure/Sysroots/2+Beta1905

CROSS_COMPILE = $(CROSS_DIR)/arm-poky-linux-musleabi-
CORTEX = -march=armv7ve -mthumb -mfpu=neon -mfloat-abi=hard -mcpu=cortex-a7

include ../../py/mkenv.mk
PYTHON = C:/Users/HP/AppData/Local/Programs/Python/Python37-32/python

QSTR_DEFS = qstrdefsport.h
include $(TOP)/py/py.mk
DFU = $(TOP)/tools/dfu.py
PYDFU = $(TOP)/tools/pydfu.py

INC += -I.
INC += -I$(TOP)
INC += -I$(BUILD)
INC += -I$(SYSROOT)/usr/include

CFLAGS  = $(CORTEX) $(INC) -B "$(CROSS_DIR)" --sysroot=$(SYSROOT) 
CFLAGS += -std=c11 -Wall -fno-omit-frame-pointer -fno-strict-aliasing -fno-exceptions -Os -D_POSIX_C_SOURCE $(COPT)
CFLAGS += -Wno-unused-value
LD = $(CROSS_COMPILE)gcc
LDFLAGS = $(CORTEX) -B "$(CROSS_DIR)" --sysroot=$(SYSROOT) -Wl,--no-undefined -nodefaultlibs
LIBS = -L$(SYSROOT)/usr/libraries -lc -lgcc_s -lapplibs -lcurl

CSUPEROPT = -Os # save some code space

SRC_C = \
	main.c \
	az_uart.c \
	py_init.c \
	py_std.c \
	py_execute.c \
	py_buffers.c \
	mod_utime.c \
	mod_device.c \
	mod_pin.c \
	mod_i2c.c \
	mod_led.c \
	mod_sock.c \
	mod_curl.c \
	lib/utils/printf.c \
	lib/utils/stdout_helpers.c \
	lib/utils/pyexec.c \
	lib/libc/string0.c \
	lib/mp-readline/readline.c \
	$(BUILD)/_frozen_mpy.c \

# List of sources for qstr extraction
SRC_QSTR += mod_utime.c
SRC_QSTR += mod_device.c
SRC_QSTR += mod_pin.c
SRC_QSTR += mod_i2c.c
SRC_QSTR += mod_led.c
SRC_QSTR += mod_sock.c
SRC_QSTR += mod_curl.c

OBJ = $(PY_CORE_O) $(addprefix $(BUILD)/, $(SRC_C:.c=.o))

all: $(BUILD)/app.elf

$(BUILD)/_frozen_mpy.c: frozentest.mpy $(BUILD)/genhdr/qstrdefs.generated.h
	$(ECHO) "MISC FREEZING BYTECODE"
	$(Q)$(TOP)/tools/mpy-tool.py -f -q $(BUILD)/genhdr/qstrdefs.preprocessed.h -mlongint-impl=none $< > $@

$(BUILD)/app.elf: $(OBJ)
	$(ECHO) "LINKING $@"
	$(Q)$(LD) $(LDFLAGS) -o $@ $^ $(LIBS)
	$(Q)$(SIZE) $@

include $(TOP)/py/mkrules.mk
