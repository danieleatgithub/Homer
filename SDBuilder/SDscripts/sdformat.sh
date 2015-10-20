#!/bin/bash
echo -n "Start MMC formatting ..."
if [ $# -ne 0 ];then
	echo "Usage: $(basename $0)"
	echo "Format a mmc in three partition according to a saved layout"
	exit 1
fi

# Save partition layout
# sfdisk -d /dev/mmcblk0 > sfdisk.8GB.layout
#

#
# TODO: Fare layout per le 16GB
#
declare -A layout
layout[7761920]=8
layout[7838720]=8 # 8GB Samsung



SF_SIZE=$(sudo sfdisk -s /dev/mmcblk0)
if ! [ ${layout[$SF_SIZE]+abcde} ];then
	echo "Unknow geometry $SF_SIZE"
	echo $RESULT
	exit 1
fi

LAYOUT_FILE="sfdisk.${layout[$SF_SIZE]}GB.layout"

# mount target devices
umount /dev/mmcblk0p1 &> /dev/null
umount /dev/mmcblk0p2 &> /dev/null
umount /dev/mmcblk0p3 &> /dev/null


dd if=/dev/zero of=/dev/mmcblk0 bs=512 count=1 &> /dev/null
if [[ $? != "0" ]];then
	echo "Unable to clean dos boot partition"
	exit 1
fi

RESULT=$(sudo sfdisk /dev/mmcblk0 < $LAYOUT_FILE 2>&1)
if [[ $? != "0" ]];then
	echo "Unable to format disk"
	exit 1
fi

RESULT=$(sudo mkdosfs -F16 -v -n "kernel" /dev/mmcblk0p1 2>&1)
if [[ $? != "0" ]];then
	echo "Unable to make boot filesystem"
	exit 1
fi

RESULT=$(sudo mkfs.ext4 -j -O extent -L "rootfs"  /dev/mmcblk0p2 2>&1)
if [[ $? != "0" ]];then
	echo "Unable to make root filesystem"
	exit 1
fi

RESULT=$(sudo mkdosfs -F32 -v -n "data" 	    	/dev/mmcblk0p3 2>&1)
if [[ $? != "0" ]];then
	echo "Unable to make data filesystem"
	exit 1
fi

RESULT=$(sudo mkswap /dev/mmcblk0p4 2>&1)
if [[ $? != "0" ]];then
	echo "Unable to make swap partition"
	exit 1
fi

echo "Done"
exit 0
