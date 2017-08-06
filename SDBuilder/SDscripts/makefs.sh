#!/bin/bash
#######################################################
#
# Filesystem builder
#
#######################################################


# source build enviroment
SPATH=$(dirname $0)
. $SPATH/../enviroment.sh

#MODE="uboot"
MODE="direct"

if [ ! -b $SDDEV ];then
	echo "Not mmc $SDDEV card present" >&2
	exit 1
fi

echo -n "Make FS Umount and clean partition ..." 
# Umount automounted partition and remount on script mount point
umount $SDDEV1 &>> $LOG
umount $SDDEV2 &>> $LOG
umount $SDDEV3 &>> $LOG
# clean mount point
rm -Rf 		$TARGET_ROOTFS_DIR 	&>> $LOG 
rm -Rf 		$TARGET_KERNEL_DIR 	&>> $LOG
rm -Rf 		$TARGET_DATA_DIR 	&>> $LOG 
mkdir -p 	$TARGET_ROOTFS_DIR 	&>> $LOG && \
mkdir -p 	$TARGET_KERNEL_DIR 	&>> $LOG && \
mkdir -p 	$TARGET_DATA_DIR 	&>> $LOG
if [[ $? !=  "0" ]];then
	echo "Failed to prepare mount points" >&2
	exit 1
fi
echo "Done"

#Setup source stuff of my build

SOURCE_ZIMAGE="/wks/buildroot-2017.02.5/output/build/linux-4.9.40/arch/arm/boot/zImage"
SOURCE_DEVTREE="/wks/buildroot-2017.02.5/output/build/linux-4.9.40/arch/arm/boot/dts/at91-sama5d3_acquaa5.dtb"
SOURCE_ROOTFS=$BR_ROOT/output/images/rootfs.tar

echo "Make FS - Start formatting" >&2
# format SD
RESULT=$($SDFORMAT)
if [[ $? !=  "0" ]];then
	echo "Failed to formatting MMC $RESULT" >&2
	exit 1
fi
echo "Make FS - Formatting completed" >&2

echo -n "Make FS - mount partition ..." >&2
# mount sd partitions
mount $SDDEV1 $TARGET_KERNEL_DIR && \
mount $SDDEV2 $TARGET_ROOTFS_DIR && \
mount $SDDEV3 $TARGET_DATA_DIR
if [[ $? !=  "0" ]];then
	echo "Failed to mount points partitions" >&2
	exit 1
fi
echo "Done" >&2


# Primary bootloader
# Copy at91 linux loader 
echo -n "Make FS - write bootloader at91 ..." >&2
SOURCE_BOOT="$AT91BINARY/sama5d3_acqua-sdcardboot-linux-zimage-dt-3.7.bin"
cp $SOURCE_BOOT $TARGET_KERNEL_DIR/linux-boot.bin
echo "Done" >&2


if [[ $MODE != "uboot" ]]; then
	echo -n "Make FS - Prepare for booting directly to linux ..." >&2
	cp $TARGET_KERNEL_DIR/linux-boot.bin $TARGET_KERNEL_DIR/boot.bin
else
	echo -n "Make FS - Prepare for booting with u-boot bootloader ..." >&2
	cp $TARGET_KERNEL_DIR/uboot-boot.bin $TARGET_KERNEL_DIR/boot.bin
	#SOURCE_UBOOT=/home/daniele/git/u-boot/
	#SOURCE_UBOOT=$BR_ROOT/output/images/
fi
echo "Done" >&2

echo -n "Make FS - copy linux kernel ..." >&2
# Copy linux kernel
cp $SOURCE_ZIMAGE $TARGET_KERNEL_DIR/.
cp $SOURCE_DEVTREE $TARGET_KERNEL_DIR/at91-sama5d3_acqua.dtb
echo "Done" >&2

# Copy root file system
echo -n "Make FS - write root filesystem ..." >&2
cd $TARGET_ROOTFS_DIR
tar -C $TARGET_ROOTFS_DIR -xpf $SOURCE_ROOTFS
if [[ $? !=  "0" ]];then
	echo "Failed copy root filesystem" >&2
	cd -
	exit 1
fi
cd - >& /dev/null
echo "Done" >&2

# Copy addons
echo -n "Make FS - copy fstab ..." >&2
cat $TARGET_ROOTFS_DIR/etc/fstab.add >> $TARGET_ROOTFS_DIR/etc/fstab
if [[ $? !=  "0" ]];then
	echo "Failed to modify fstab"
	exit 1
fi
rm -f $TARGET_ROOTFS_DIR/etc/fstab.add
echo "Done" >&2


echo "Make FS - ----- Root Home -----" >&2
ls $TARGET_ROOTFS_DIR/root >&2
echo "Make FS - ---------------------" >&2

echo -n "Make FS - Unmounting FS ..." >&2
umount $TARGET_ROOTFS_DIR && \
umount $TARGET_KERNEL_DIR && \
umount $TARGET_DATA_DIR
if [[ $? !=  "0" ]];then
	echo "Failed to umount partitions" >&2
	exit 1
fi
echo "Make FS - Done" >&2



