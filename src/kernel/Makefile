TARGET_ASMFLAGS += -f elf
TARGET_CFLAGS += -ffreestanding -nostdlib -I $(SRC_DIR)/libs -I. -Iarch/i686 -Wall -Wextra
TARGET_LIBS += -lgcc
TARGET_LINKFLAGS += -T linker.ld -nostdlib

HEADERS_C = $(shell find . -type f -name "*.h") \
			$(shell find $(SRC_DIR)/libs/boot -type f -name "*.h")
SOURCES_C = $(shell find . -type f -name "*.c")
HEADERS_ASM=$(shell find . -type f -name "*.inc") \
			arch/i686/isrs_gen.inc
SOURCES_ASM=$(shell find . -type f -name "*.asm")
OBJECTS_C=$(patsubst %.c, $(BUILD_DIR)/kernel/c/%.obj, $(SOURCES_C))
OBJECTS_ASM=$(patsubst %.asm, $(BUILD_DIR)/kernel/asm/%.obj, $(SOURCES_ASM))

.PHONY: all kernel clean always

all: kernel

kernel: $(BUILD_DIR)/kernel.bin

$(BUILD_DIR)/kernel.bin: $(OBJECTS_ASM) $(OBJECTS_C) 
	@$(TARGET_LD) $(TARGET_LINKFLAGS) -Wl,-Map=$(BUILD_DIR)/kernel.map -o $@ $^ $(TARGET_LIBS)
	@echo "--> Created:  kernel.bin"

$(BUILD_DIR)/kernel/c/%.obj: %.c $(HEADERS_C)
	@mkdir -p $(@D)
	@$(TARGET_CC) $(TARGET_CFLAGS) -c -o $@ $<
	@echo "--> Compiled: " $<

$(BUILD_DIR)/kernel/asm/%.obj: %.asm $(HEADERS_ASM)
	@mkdir -p $(@D)
	@$(TARGET_ASM) $(TARGET_ASMFLAGS) -o $@ $<
	@echo "--> Compiled: " $<

arch/i686/isrs_gen.c arch/i686/isrs_gen.inc:
	@$(SOURCE_DIR)/build_scripts/generate_isrs.sh $(SRC_DIR)/kernel/arch/i686/isrs_gen.c $(SRC_DIR)/kernel/arch/i686/isrs_gen.inc

clean:
	rm -f $(BUILD_DIR)/kernel.bin