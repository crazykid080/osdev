ASM=nasm

SRC_DIR=src
BUILD_DIR=build


.PHONY: all floppy_image kernel bootloader clean always


#
# Floppy disk size
#
floppy_image: bootloader kernel
$(BUILD_DIR)/main_floppy.img: $(BUILD_DIR)/kernel.bin
	cp $(BUILD_DIR)/kernel.bin $(BUILD_DIR)/main.img
	truncate -s 1440k $(BUILD_DIR)/main.img

#
# Bootloader
#
bootloader: $(BUILD_DIR)/bootloader.bin: always
	$(ASM) $(SRC_DIR)/bootloader/boot.asm -f bin -o $(BUILD_DIR)/bootloader.bin

#
# Kernel
#
kernel: $(BUILD_DIR)/kernel.bin
$(BUILD_DIR)/kernel.bin: always
	$(ASM) $(SRC_DIR)/kernel/main.asm -f bin -o $(BUILD_DIR)/kernel.bin


#
# always
#
always:
	mkdir -p $(BUILD_DIR)

#
# clean
#
clean:
	mr -rf $(BUILD_DIR)/*