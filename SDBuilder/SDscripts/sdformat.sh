#!/bin/bash

SDDEV="/dev/sdc"
SDDEV1="/dev/sdc1"
SDDEV2="/dev/sdc2"
SDDEV3="/dev/sdc3"
SDDEV4="/dev/sdc4"
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



SF_SIZE=$(sudo sfdisk -s $SDDEV)
if ! [ ${layout[$SF_SIZE]+abcde} ];then
	echo "Unknow geometry $SF_SIZE"
	echo $RESULT
	exit 1
fi

LAYOUT_FILE="sfdisk.${layout[$SF_SIZE]}GB.layout"

# mount target devices
umount $SDDEV1 &> /dev/null
umount $SDDEV2 &> /dev/null
umount $SDDEV3 &> /dev/null


dd if=/dev/zero of=$SDDEV bs=512 count=1 &> /dev/null
if [[ $? != "0" ]];then
	echo "Unable to clean dos boot partition"
	exit 1
fi

RESULT=$(sudo sfdisk $SDDEV < $LAYOUT_FILE 2>&1)
if [[ $? != "0" ]];then
	echo "Unable to format disk"
	exit 1
fi

RESULT=$(sudo mkdosfs -F16 -v -n "kernel" $SDDEV1 2>&1)
if [[ $? != "0" ]];then
	echo "Unable to make boot filesystem"
	exit 1
fi

RESULT=$(sudo mkfs.ext4 -j -O extent -L "rootfs"  $SDDEV2 2>&1)
if [[ $? != "0" ]];then
	echo "Unable to make root filesystem"
	exit 1
fi

RESULT=$(sudo mkdosfs -F32 -v -n "data" 	    	$SDDEV3 2>&1)
if [[ $? != "0" ]];then
	echo "Unable to make data filesystem"
	exit 1
fi

RESULT=$(sudo mkswap $SDDEV4 2>&1)
if [[ $? != "0" ]];then
	echo "Unable to make swap partition"
	exit 1
fi

echo "Done"
exit 0
