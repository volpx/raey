# Makefile for AVR C++ projects

# ----- Update the settings of your project here -----

# use spaces for indentation
#.RECIPEPREFIX +=

.PHONY = all clean build flash config help default

# Hardware
MCU     = atmega328p # see `make show-mcu`
MCU_ID  = m328p
OSC     = 16000000UL
PROJECT = raey
TARGET  = main

SRC_DIR  = src
INC_DIR  = include
BUILD_DIR=build

# ----- These configurations are quite likely not to be changed -----

# Binaries
GCC     = avr-gcc
G++     = avr-g++
RM      = rm -f
AVRDUDE = avrdude
OBJCOPY = avr-objcopy

# Files
EXT_C   = c
EXT_C++ = cpp
EXT_ASM = asm

OBJ_DIR  = $(BUILD_DIR)/obj
BIN_DIR  = $(BUILD_DIR)/bin

# ----- No changes should be necessary below this line -----

SRCS_C   = $(wildcard $(SRC_DIR)/*.$(EXT_C))
SRCS_C++ = $(wildcard $(SRC_DIR)/*.$(EXT_C++))
SRCS_C++ += $(TARGET).cpp
SRC_ASM  = $(wildcard $(SRC_DIR)/*.$(EXT_ASM))

# list the objects
OBJECTS = \
	$(patsubst %.$(EXT_C),$(OBJ_DIR)/%.o,$(notdir $(SRCS_C))) \
	$(patsubst %.$(EXT_C++),$(OBJ_DIR)/%.o,$(notdir $(SRCS_C++))) \
	$(patsubst %.$(EXT_ASM),$(OBJ_DIR)/%.o,$(notdir $(SRCS_ASM)))

ELF = $(BIN_DIR)/$(TARGET).out

# TODO explain these flags, make them configurable
LINKERFLAG=

CFLAGS = -I$(INC_DIR)
CFLAGS += -Os
CFLAGS += -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
CFLAGS += -Wall -Wstrict-prototypes -Wshadow
CFLAGS += -DF_OSC=$(OSC)
CFLAGS += -mmcu=$(MCU)

C++FLAGS = -I$(INC_DIR)
C++FLAGS += -Os
C++FLAGS += -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
C++FLAGS += -Wall -Wshadow
C++FLAGS += -DF_OSC=$(OSC)
C++FLAGS += -mmcu=$(MCU)

ASMFLAGS = -I$(INC_DIR)
ASMFLAGS += -Os
ASMFLAGS += -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
ASMFLAGS += -Wall -Wstrict-prototypes
ASMFLAGS += -DF_OSC=$(OSC)
ASMFLAGS += -x assembler-with-cpp
ASMFLAGS += -mmcu=$(MCU)

# Attempt to create the output directories
ifneq ($(BUILD_DIR),)
$(shell [ -d $(BUILD_DIR) ] || mkdir -p $(BUILD_DIR))
$(shell [ -d $(OBJ_DIR) ] || mkdir -p $(OBJ_DIR))
$(shell [ -d $(BIN_DIR) ] || mkdir -p $(BIN_DIR))
endif

all: clean build flash
	@echo "Done"
foo:
	@echo $(SRCS_C)
	@echo $(SRCS_C++)
	@echo $(SRCS_ASM)
	@echo $(OBJECTS)

$(BUILD_DIR)/$(PROJECT).elf: $(OBJECTS)
	@echo $@
	# Link alltogether
	#$(GCC) $(CFLAGS) $(OBJECTS) --output $@ $(LDFLAGS)
	$(GCC) $(CFLAGS) $^ --output $@ $(LDFLAGS)

# Compile the files
$(OBJ_DIR)/%.o : $(SRC_DIR)/%.$(EXT_C)
	$(GCC) $< $(CFLAGS) -c -o $@

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.$(EXT_C++)
	$(G++) $< $(C++FLAGS) -c -o $@

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.$(EXT_ASM)
	$(G++) $< $(ASMFLAGS) -c -o $@


help:
	@echo "usage:"
	@echo "  make <target>"
	@echo ""
	@echo "targets:"
	@echo "  clean     Remove any non-source files"
	@echo "  config    Shows the current configuration"
	@echo "  help      Shows this help"
	@echo "  show-mcu  Show list of all possible MCUs"

config:
	@echo "configuration:"
	@echo ""
	@echo "Binaries for:"
	@echo "  C compiler:   $(GCC)"
	@echo "  C++ compiler: $(G++)"
	@echo "  Programmer:   $(AVRDUDE)"
	@echo "  remove file:  $(RM)"
	@echo ""
	@echo "Hardware settings:"
	@echo "  MCU: $(MCU)"
	@echo "  OSC: $(OSC)"
	@echo ""
	@echo "Defaults:"
	@echo "  C-files:   *.$(EXT_C)"
	@echo "  C++-files: *.$(EXT_C++)"
	@echo "  ASM-files: *.$(EXT_ASM)"

build: $(BUILD_DIR)/$(PROJECT).elf
	${OBJCOPY} -j .text -j .data -O ihex $< $(BUILD_DIR)/${TARGET}.hex

flash:
	avrdude -p $(MCU_ID) -c usbtiny -U flash:w:$(TARGET).hex:i -F -P usb

clean:
	rm -rf $(BUILD_DIR) || true

show-mcu:
	$(G++) --help=target

debug:
	echo $(TARGET)
