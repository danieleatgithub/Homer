#!/bin/bash

BR_ROOT="/wks/buildroot-2015.08.1"
MMC="/dev/mmcblk0"
#MODE="uboot"
MODE="direct"
ROOT="/wks"
WORKSPACE="$ROOT/workspace"
TARGET_HOMER="$WORKSPACE/Homer/SDBuilder/target"

if [ $# -ne 1 ];then
	echo "Usage: sudo $(basename $0) Target [Homer|Margie]"
	exit 1
fi
if [ ! -b $MMC ];then
	echo "Not mmc $MMC card present"
	exit 1
fi

CONF=$1.conf
if [ ! -r $HOST_HOMER/$CONF ];then
	echo "Usage: sudo $(basename $0) Target[Homer|Margie]"
	echo "Error: Configuration file $1.conf not exists"
	exit 1
fi

TARGET_ROOTFS_DIR=/media/rootfs
TARGET_KERNEL_DIR=/media/kernel
TARGET_DATA_DIR=/media/data

# Umount automounted partition and remount on script mount point
umount /dev/mmcblk0p1 &> /dev/null 
umount /dev/mmcblk0p2 &> /dev/null
umount /dev/mmcblk0p3 &> /dev/null

rm -Rf $TARGET_ROOTFS_DIR &> /dev/null
rm -Rf $TARGET_KERNEL_DIR &> /dev/null
rm -Rf $TARGET_DATA_DIR &> /dev/null

mkdir -p $TARGET_ROOTFS_DIR &> /dev/null && \
mkdir -p $TARGET_KERNEL_DIR &> /dev/null && \
mkdir -p $TARGET_DATA_DIR &> /dev/null
if [[ $? !=  "0" ]];then
	echo "Failed to prepare mount points"
	exit 1
fi


#Setup source stuff of my build
SOURCE_ZIMAGE=/wks/linux-3.16.1/arch/arm/boot/zImage
SOURCE_DEVTREE=/wks/linux-3.16.1/arch/arm/boot/dts/acme-acqua.dtb
SOURCE_ROOTFS=$BR_ROOT/output/images/rootfs.tar

/wks/workspace/Homer/SDBuilder/SDscripts/sdformat.sh
if [[ $? !=  "0" ]];then
	echo "Failed to formatting MMC"
	exit 1
fi

mount /dev/mmcblk0p1 $TARGET_KERNEL_DIR && \
mount /dev/mmcblk0p2 $TARGET_ROOTFS_DIR && \
mount /dev/mmcblk0p3 $TARGET_DATA_DIR
if [[ $? !=  "0" ]];then
	echo "Failed to mount points partitions"
	exit 1
fi


# Primary bootloader
# Copy at91 linux loader 
SOURCE_BOOT=/wks/workspace/at91bootstrap/binaries/sama5d3_acqua-sdcardboot-linux-zimage-dt-3.7.bin
cp $SOURCE_BOOT $TARGET_KERNEL_DIR/linux-boot.bin

# Copy at91 linux loader 
SOURCE_BOOT=/wks/workspace/at91bootstrap/binaries/sama5d3_acqua-sdcardboot-uboot-3.7.bin
SOURCE_UBOOT=/wks/workspace/u-boot-at91/
#cp $SOURCE_UBOOT/boot.bin $TARGET_KERNEL_DIR/uboot-boot.bin
#cp $SOURCE_UBOOT/u-boot.bin $TARGET_KERNEL_DIR/u-boot.bin
#cp $SOURCE_UBOOT/u-boot.img $TARGET_KERNEL_DIR/u-boot.img
#cp $SOURCE_UBOOT/u-boot.map $TARGET_KERNEL_DIR/u-boot.map
#cp ./uboot.env $TARGET_KERNEL_DIR/.


if [[ $MODE != "uboot" ]]; then
	echo "Prepare for booting directly to linux"
	cp $TARGET_KERNEL_DIR/linux-boot.bin $TARGET_KERNEL_DIR/boot.bin
else
	echo "Prepare for booting with u-boot bootloader"
	cp $TARGET_KERNEL_DIR/uboot-boot.bin $TARGET_KERNEL_DIR/boot.bin
	#SOURCE_UBOOT=/home/daniele/git/u-boot/
	#SOURCE_UBOOT=$BR_ROOT/output/images/
fi


# Copy linux kernel
cp $SOURCE_ZIMAGE $TARGET_KERNEL_DIR/.
cp $SOURCE_DEVTREE $TARGET_KERNEL_DIR/at91-sama5d3_acqua.dtb

# Copy root file system
cd $TARGET_ROOTFS_DIR
tar -C $TARGET_ROOTFS_DIR -xpf $SOURCE_ROOTFS
if [[ $? !=  "0" ]];then
	echo "Failed copy root filesystem"
	cd -
	exit 1
fi
cd -

# Setup fstab
cat $TARGET_ROOTFS_DIR/etc/fstab.add >> $TARGET_ROOTFS_DIR/etc/fstab
if [[ $? !=  "0" ]];then
	echo "Failed to modify fstab"
	exit 1
fi
rm -f $TARGET_ROOTFS_DIR/etc/fstab.add

# Copy conf file
cp $TARGET_HOMER/$CONF $TARGET_ROOTFS_DIR/etc/system.conf

echo "Unmounting FS"
umount $TARGET_ROOTFS_DIR && \
umount $TARGET_KERNEL_DIR && \
umount $TARGET_DATA_DIR
if [[ $? !=  "0" ]];then
	echo "Failed to umount partitions"
	exit 1
fi


echo "Done"
