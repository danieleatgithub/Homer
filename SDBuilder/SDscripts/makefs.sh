#!/bin/bash
#######################################################
#
# Filesystem builder
#
#######################################################

. /wks/workspace/Homer/homer_deploy/environment.sh
. ${SD_BUILDER}/SDscripts/layouts.sh
. ${SD_BUILDER}/SDscripts/devices.sh

if [ ! -b $SDDEV ];then
	echo "Not mmc $SDDEV card present" >&2
	exit 1
fi

echo -n "Make FS Umount and clean partition ..." 
# Umount automounted partition and remount on script mount point
umount $SDDEV1 &>> $LOG
umount $SDDEV2 &>> $LOG
umount $SDDEV3 &>> $LOG

ROOTFS=/media/rootfs
BOOT=/media/boot
DATA=/media/data

# clean mount point
rm -Rf 		$ROOTFS &>> $LOG 
rm -Rf 		$BOOT  	&>> $LOG
rm -Rf 		$DATA 	&>> $LOG 
mkdir -p 	$ROOTFS &>> $LOG && \
mkdir -p 	$BOOT 	&>> $LOG && \
mkdir -p 	$DATA 	&>> $LOG
if [[ $? !=  "0" ]];then
	echo "Failed to prepare mount points" >&2
	exit 1
fi
echo "Done"

#Setup source stuff of my build
SOURCE_ZIMAGE=${BUILDROOT}/output/build/linux-${LINUX_VER}/arch/arm/boot/zImage
SOURCE_DTB=${BUILDROOT}/output/build/linux-${LINUX_VER}/arch/arm/boot/dts/${DEVTREE}.dtb
SOURCE_ROOTFS=${BUILDROOT}/output/images/rootfs.tar

echo "Make FS - Start formatting" >&2
# format SD
RESULT=$(${SD_BUILDER}/SDscripts/sdformat.sh)
if [[ $? !=  "0" ]];then
	echo "Failed to formatting MMC $RESULT" >&2
	exit 1
fi
echo "Make FS - Formatting completed" >&2

echo -n "Make FS - mount partition ..." >&2
# mount sd partitions
mount $SDDEV1 $BOOT && \
mount $SDDEV2 $ROOTFS && \
mount $SDDEV3 $DATA
if [[ $? !=  "0" ]];then
	echo "Failed to mount points partitions" >&2
	exit 1
fi
echo "Done" >&2


# Primary bootloader
# Copy at91 linux loader 
echo -n "Make FS - write bootloader at91 ..." >&2
cp ${AT91_ROOT}/binaries/sama5d3_acqua-sdcardboot-linux-zimage-dt-3.7.bin ${BOOT}/boot.bin
echo "Done" >&2

echo -n "Make FS - copy linux kernel ..." >&2
# Copy linux kernel
cp ${SOURCE_ZIMAGE} ${BOOT}/.
cp ${SOURCE_DTB} 	${BOOT}/at91-sama5d3_acqua.dtb
echo "Done" >&2

# Copy root file system
echo -n "Make FS - write root filesystem ..." >&2
cd $ROOTFS
tar -C $ROOTFS -xpf $SOURCE_ROOTFS
if [[ $? !=  "0" ]];then
	echo "Failed copy root filesystem" >&2
	cd -
	exit 1
fi
cd - >& /dev/null
echo "Done" >&2

echo "Make FS - ----- Root Home -----" >&2
ls $ROOTFS/root >&2
echo "Make FS - ---------------------" >&2

stdbuf -o0 echo -n "Make FS - Unmounting FS ..." >&2
umount $ROOTFS && \
umount $BOOT && \
umount $DATA
if [[ $? !=  "0" ]];then
	echo "Make FS - Failed to umount partitions" >&2
	exit 1
fi
echo "Done" >&2




