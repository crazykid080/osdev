#!/bin/bash
set -e

TARGET=$1
SIZE=$2

STAGE1_STAGE2_OFFSET=480
DISK_PART1_BEGIN=2048

#generate image file
DISK_SECTOR_COUNT=$(( (${SIZE} + 511 ) / 512 ))

dd if=/dev/zero of=$TARGET bs=512 count=${DISK_SECTOR_COUNT} > /dev/null

echo "Writing partition table...."
# create partition table
parted -s $TARGET mklabel msdos
DISK_SECTOR_END=$(( ${DISK_SECTOR_COUNT} - 1))
parted -s $TARGET mkpart primary ${DISK_PART1_BEGIN}s ${DISK_SECTOR_END}s
parted -s $TARGET set 1 boot on

#calculate needed number of reserved sectors
STAGE2_SIZE=$(stat -c%s ${BUILD_DIR}/stage2.bin)
STAGE2_SECTORS=$(( (${STAGE2_SIZE}+511)/512 ))
RESERVED_SECTORS=$(( 1 + ${STAGE2_SECTORS}))

if [ ${STAGE2_SECTORS} \>  $(( ${DISK_PART1_BEGIN} - 1 )) ]; then
    echo "Stage 2 too large for disk!!"
    exit 2
fi

echo "Writing stage 2...."
# write stage 2 to disk
dd if=${BUILD_DIR}/stage2.bin of=$TARGET conv=notrunc bs=512 seek=1 > /dev/null

echo "Creating loopback device..."
# create loopback device. 
DEVICE=$(losetup -fP --show ${TARGET})
TARGET_PARTITION="${DEVICE}p1"
echo "Created ${DEVICE}"

#create file system
mkfs.fat -n "ASYOS" $TARGET_PARTITION > /dev/null

#install bootloader
echo "Installing bootloader..."
dd if=${BUILD_DIR}/stage1.bin of=$TARGET conv=notrunc bs=1 count=3 2>&1 > /dev/null
dd if=${BUILD_DIR}/stage1.bin of=$TARGET conv=notrunc bs=1 skip=90 seek=90 2>&1 > /dev/null

# write lba address of stage 2 to bootloader
echo "01 00 00 00" | xxd -r -p | dd of=$TARGET conv=notrunc bs=1 seek=${STAGE1_STAGE2_OFFSET}
printf "%x" ${STAGE2_SECTORS} | xxd -r -p | dd of=$TARGET conv=notrunc bs=1 seek=$((STAGE1_STAGE2_OFFSET+4))

#copy files
mkdir -p /tmp/asyos
mount ${TARGET_PARTITION} /tmp/asyos
cp ${BUILD_DIR}/kernel.bin /tmp/asyos
cp test.txt /tmp/asyos
mkdir /tmp/asyos/test
cp test.txt /tmp/asyos/test
umount /tmp/asyos

# destroy loopback device
losetup -d ${DEVICE}