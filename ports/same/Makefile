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

INC += -I.
INC += -I$(TOP)
INC += -I$(BUILD)
INC += -I$(BOARD_DIR)
INC += -I$(TOP)/lib/cmsis/inc
INC += -I$(TOP)/lib/asf4/$(shell echo $(MCU_SERIES) | tr '[:upper:]' '[:lower:]')/
INC += -I$(TOP)/lib/asf4/$(shell echo $(MCU_SERIES) | tr '[:upper:]' '[:lower:]')/CMSIS/Core/Include
INC += -I$(TOP)/lib/asf4/$(shell echo $(MCU_SERIES) | tr '[:upper:]' '[:lower:]')/config
INC += -I$(TOP)/lib/asf4/$(shell echo $(MCU_SERIES) | tr '[:upper:]' '[:lower:]')/examples
INC += -I$(TOP)/lib/asf4/$(shell echo $(MCU_SERIES) | tr '[:upper:]' '[:lower:]')/hal/include
INC += -I$(TOP)/lib/asf4/$(shell echo $(MCU_SERIES) | tr '[:upper:]' '[:lower:]')/hal/utils/include
INC += -I$(TOP)/lib/asf4/$(shell echo $(MCU_SERIES) | tr '[:upper:]' '[:lower:]')/hpl/core
INC += -I$(TOP)/lib/asf4/$(shell echo $(MCU_SERIES) | tr '[:upper:]' '[:lower:]')/hpl/mcan
INC += -I$(TOP)/lib/asf4/$(shell echo $(MCU_SERIES) | tr '[:upper:]' '[:lower:]')/hpl/pio
INC += -I$(TOP)/lib/asf4/$(shell echo $(MCU_SERIES) | tr '[:upper:]' '[:lower:]')/hpl/pmc
INC += -I$(TOP)/lib/asf4/$(shell echo $(MCU_SERIES) | tr '[:upper:]' '[:lower:]')/hpl/uart
INC += -I$(TOP)/lib/asf4/$(shell echo $(MCU_SERIES) | tr '[:upper:]' '[:lower:]')/hpl/usbhs
INC += -I$(TOP)/lib/asf4/$(shell echo $(MCU_SERIES) | tr '[:upper:]' '[:lower:]')/hpl/xdmac
INC += -I$(TOP)/lib/asf4/$(shell echo $(MCU_SERIES) | tr '[:upper:]' '[:lower:]')/hri
INC += -I$(TOP)/lib/asf4/$(shell echo $(MCU_SERIES) | tr '[:upper:]' '[:lower:]')/same70b/include
INC += -I$(TOP)/lib/asf4/$(shell echo $(MCU_SERIES) | tr '[:upper:]' '[:lower:]')/usb
INC += -I$(TOP)/lib/asf4/$(shell echo $(MCU_SERIES) | tr '[:upper:]' '[:lower:]')/usb/class/cdc
INC += -I$(TOP)/lib/asf4/$(shell echo $(MCU_SERIES) | tr '[:upper:]' '[:lower:]')/usb/class/cdc/device
INC += -I$(TOP)/lib/asf4/$(shell echo $(MCU_SERIES) | tr '[:upper:]' '[:lower:]')/usb/device


CFLAGS_MCU_SAME70 = -mtune=cortex-m7 -mcpu=cortex-m7 -mfpu=fpv4-sp-d16 -mfloat-abi=softfp
CFLAGS = $(INC) -Wall -Werror -std=c99 -nostdlib -mthumb $(CFLAGS_MCU_$(MCU_SERIES)) -fsingle-precision-constant -Wdouble-promotion
CFLAGS += -DMCU_$(MCU_SERIES) -D__$(CMSIS_MCU)__
LDFLAGS = -nostdlib $(addprefix -T,$(LD_FILES)) -Map=$@.map --cref
LIBS = $(shell $(CC) $(CFLAGS) -print-libgcc-file-name)

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
	lib/libc/string0.c \
	lib/libm/ef_sqrt.c \
	lib/libm/fmodf.c \
	lib/libm/math.c \
	lib/libm/nearbyintf.c \
	lib/mp-readline/readline.c \
	lib/utils/printf.c \
	lib/utils/pyexec.c \
	lib/utils/stdout_helpers.c \
	lib/utils/gchelper_m7.s

# List of sources for qstr extraction
SRC_QSTR += modutime.c modmachine.c

OBJ += $(PY_O)
OBJ += $(addprefix $(BUILD)/, $(SRC_C:.c=.o))
OBJ += $(addprefix $(BUILD)/, $(SRC_S:.s=.o))

all: $(BUILD)/firmware.uf2

$(BUILD)/firmware.elf: $(OBJ)
	$(ECHO) "LINK $@"
	$(Q)$(LD) $(LDFLAGS) -o $@ $^ $(LIBS)
	$(Q)$(SIZE) $@

$(BUILD)/firmware.bin: $(BUILD)/firmware.elf
	$(Q)$(OBJCOPY) -O binary -j .isr_vector -j .text -j .data $^ $(BUILD)/firmware.bin

$(BUILD)/firmware.uf2: $(BUILD)/firmware.bin
	$(Q)$(PYTHON) $(UF2CONV) -b $(TEXT0) -c -o $@ $<

include $(TOP)/py/mkrules.mk
