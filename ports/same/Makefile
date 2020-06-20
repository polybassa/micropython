BOARD ?= EMB60
BOARD_DIR ?= boards/$(BOARD)
BUILD ?= build-$(BOARD)

CROSS_COMPILE ?= arm-none-eabi-
UF2CONV ?= $(TOP)/tools/uf2conv.py

ifeq ($(wildcard $(BOARD_DIR)/.),)
$(error Invalid BOARD specified: $(BOARD_DIR))
endif

include ../../py/mkenv.mk
include $(BOARD_DIR)/mpconfigboard.mk

# Qstr definitions (must come before including py.mk)
QSTR_DEFS = qstrdefsport.h
QSTR_GLOBAL_DEPENDENCIES = $(BOARD_DIR)/mpconfigboard.h

# Include py core make definitions
include $(TOP)/py/py.mk

GIT_SUBMODULES = lib/asf4

LIB_PATH ?= lib/asf4/$(shell echo $(MCU_SERIES) | tr '[:upper:]' '[:lower:]')
LIB_DIR ?= $(TOP)/$(LIB_PATH)

INC += -I.
INC += -I$(TOP)
INC += -I$(BUILD)
INC += -I$(BOARD_DIR)
INC += -I$(TOP)/lib/cmsis/inc
INC += -I$(LIB_DIR)/
INC += -I$(LIB_DIR)/CMSIS/Core/Include
INC += -I$(LIB_DIR)/config
INC += -I$(LIB_DIR)/examples
INC += -I$(LIB_DIR)/hal/include
INC += -I$(LIB_DIR)/hal/utils/include
INC += -I$(LIB_DIR)/hpl/core
INC += -I$(LIB_DIR)/hpl/mcan
INC += -I$(LIB_DIR)/hpl/pio
INC += -I$(LIB_DIR)/hpl/pmc
INC += -I$(LIB_DIR)/hpl/uart
INC += -I$(LIB_DIR)/hpl/usbhs
INC += -I$(LIB_DIR)/hpl/xdmac
INC += -I$(LIB_DIR)/hri
INC += -I$(LIB_DIR)/same70b/include
INC += -I$(LIB_DIR)/usb
INC += -I$(LIB_DIR)/usb/class/cdc
INC += -I$(LIB_DIR)/usb/class/cdc/device
INC += -I$(LIB_DIR)/usb/device


CFLAGS_MCU_SAME70 = -mtune=cortex-m7 -mcpu=cortex-m7 -mfpu=fpv4-sp-d16 -mfloat-abi=softfp
CFLAGS = $(INC) -Wall -std=c99 -mthumb $(CFLAGS_MCU_$(MCU_SERIES)) -fsingle-precision-constant -Wdouble-promotion --specs=nano.specs 
CFLAGS += -DMCU_$(MCU_SERIES) -D__$(CMSIS_MCU)__
LDFLAGS = $(addprefix -T,$(LD_FILES)) -Map=$@.map --cref
LIBS = $(shell $(CC) $(CFLAGS) -print-libgcc-file-name)

DEBUG = 1

# Tune for Debugging or Optimization
ifeq ($(DEBUG),1)
CFLAGS += -O0 -ggdb
else
CFLAGS += -Os -DNDEBUG
LDFLAGS += --gc-sections
CFLAGS += -fdata-sections -ffunction-sections
endif

SRC_C = \
	main.c \
	modutime.c \
	modmachine.c \
	mphalport.c \
	same_isr.c \
	RingBuf.c \
	usb_micropython.c \
	lib/libc/string0.c \
	lib/libm/ef_sqrt.c \
	lib/libm/fmodf.c \
	lib/libm/math.c \
	lib/libm/nearbyintf.c \
	lib/mp-readline/readline.c \
	lib/utils/printf.c \
	lib/utils/pyexec.c \
	lib/utils/stdout_helpers.c \
	$(LIB_PATH)/atmel_start.o \
	$(LIB_PATH)/driver_init.o \
	$(LIB_PATH)/examples/driver_examples.o \
	$(LIB_PATH)/hal/src/hal_atomic.o \
	$(LIB_PATH)/hal/src/hal_can_async.o \
	$(LIB_PATH)/hal/src/hal_delay.o \
	$(LIB_PATH)/hal/src/hal_gpio.o \
	$(LIB_PATH)/hal/src/hal_init.o \
	$(LIB_PATH)/hal/src/hal_io.o \
	$(LIB_PATH)/hal/src/hal_sleep.o \
	$(LIB_PATH)/hal/src/hal_usart_sync.o \
	$(LIB_PATH)/hal/src/hal_usb_device.o \
	$(LIB_PATH)/hal/utils/src/utils_assert.o \
	$(LIB_PATH)/hal/utils/src/utils_event.o \
	$(LIB_PATH)/hal/utils/src/utils_list.o \
	$(LIB_PATH)/hal/utils/src/utils_ringbuffer.o \
	$(LIB_PATH)/hal/utils/src/utils_syscalls.o \
	$(LIB_PATH)/hpl/core/hpl_core_m7_base.o \
	$(LIB_PATH)/hpl/core/hpl_init.o \
	$(LIB_PATH)/hpl/mcan/hpl_mcan.o \
	$(LIB_PATH)/hpl/pmc/hpl_pmc.o \
	$(LIB_PATH)/hpl/pmc/hpl_sleep.o \
	$(LIB_PATH)/hpl/uart/hpl_uart.o \
	$(LIB_PATH)/hpl/usbhs/hpl_usbhs.o \
	$(LIB_PATH)/hpl/xdmac/hpl_xdmac.o \
	$(LIB_PATH)/same70b/gcc/gcc/startup_same70n21b.o \
	$(LIB_PATH)/same70b/gcc/system_same70n21b.o \
	$(LIB_PATH)/usb/class/cdc/device/cdcdf_acm.o \
	$(LIB_PATH)/usb/device/usbdc.o \
	$(LIB_PATH)/usb/usb_protocol.o 



SRC_S = lib/utils/gchelper_m3.s 


# List of sources for qstr extraction
SRC_QSTR += modutime.c modmachine.c

OBJ += $(PY_O)
OBJ += $(addprefix $(BUILD)/, $(SRC_C:.c=.o))
OBJ += $(addprefix $(BUILD)/, $(SRC_S:.s=.o))

all: $(BUILD)/firmware.uf2 $(BUILD)/firmware.hex

$(BUILD)/firmware.elf: $(OBJ)
	$(ECHO) "LINK $@"
	$(Q)$(LD) $(LDFLAGS) -o $@ $^ $(LIBS)
	$(Q)$(SIZE) $@

$(BUILD)/firmware.bin: $(BUILD)/firmware.elf
	$(Q)$(OBJCOPY) -O binary -j .isr_vector -j .text -j .data $^ $(BUILD)/firmware.bin

$(BUILD)/firmware.hex: $(BUILD)/firmware.elf
	$(Q)$(OBJCOPY) -O ihex -R .eeprom -R .fuse -R .lock -R .signature $^ $(BUILD)/firmware.hex

$(BUILD)/firmware.uf2: $(BUILD)/firmware.bin
	$(Q)$(PYTHON) $(UF2CONV) -b $(TEXT0) -c -o $@ $<

include $(TOP)/py/mkrules.mk
