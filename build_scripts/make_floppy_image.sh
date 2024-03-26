#!/bin/bash
TARGET=$1
STAGE1_STAGE2_OFFSET=480
#generate image file
dd if=/dev/zero of=$TARGET bs=512 count=2880 > /dev/null

#calculate needed number of reserved sectors
STAGE2_SIZE=$(stat -c%s ${BUILD_DIR}/stage2.bin)
STAGE2_SECTORS=$(( (${STAGE2_SIZE}+511)/512 ))
RESERVED_SECTORS=$(( 1 + ${STAGE2_SECTORS}))

#create file system
mkfs.fat -F 12 -R ${RESERVED_SECTORS} -n "NBOS" $TARGET > /dev/null

#install bootloader
dd if=${BUILD_DIR}/stage1.bin of=$TARGET conv=notrunc bs=1 count=3 > /dev/null
dd if=${BUILD_DIR}/stage1.bin of=$TARGET conv=notrunc bs=1 skip=62 seek=62 > /dev/null
dd if=${BUILD_DIR}/stage2.bin of=$TARGET conv=notrunc bs=512 seek=1 > /dev/null

# write lba address of stage 2 to bootloader
echo "01 00 00 00" | xxd -r -p | dd of=$TARGET conv=notrunc bs=1 seek=${STAGE1_STAGE2_OFFSET}
printf "%x" ${STAGE2_SECTORS} | xxd -r -p | dd of=$TARGET conv=notrunc bs=1 seek=$((STAGE1_STAGE2_OFFSET+4))

#copy files
mcopy -i $TARGET ${BUILD_DIR}/kernel.bin "::kernel.bin"
mcopy -i $TARGET test.txt "::test.txt"
mmd -i $TARGET "::mydir"
mcopy -i $TARGET dirtest.txt "::mydir/test.txt"