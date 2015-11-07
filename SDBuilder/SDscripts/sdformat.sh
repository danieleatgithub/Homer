#!/bin/bash
#######################################################
#
# MMC Formatter
#
#######################################################

# source build enviroment
SPATH=$(dirname $0)
. $SPATH/../enviroment.sh

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
layout[3866624]=4 # 4GB 
#Disk identifier: 0xc4814c7a
#Device     Boot   Start     End Sectors  Size Id Type
#/dev/sdd1          2048 1021952 1019905  498M  6 FAT16
#/dev/sdd2       1024000 3121151 2097152    1G 83 Linux
#/dev/sdd3       3121152 7315455 4194304    2G  c W95 FAT32 (LBA)
#/dev/sdd4       7315456 7733247  417792  204M 82 Linux swap / Solaris

# find layout
echo -n "SD formatting prepare layout ..."
SF_SIZE=$(sudo sfdisk -s $SDDEV)
if ! [ ${layout[$SF_SIZE]+abcde} ];then
	echo "Unknow geometry $SF_SIZE"
	echo $RESULT
	exit 1
fi
# prepare SD layout
LAYOUT_FILE_TEMPLATE=${SDSCRIPTS}/"sfdisk.${layout[$SF_SIZE]}GB.layout"
LAYOUT_FILE="/tmp/layout"
cp $LAYOUT_FILE_TEMPLATE $LAYOUT_FILE
sed -i "s/__DEVICE__/${DEVICE}/g" $LAYOUT_FILE
echo "Done"

# mount target devices
umount $SDDEV1 &> /dev/null
umount $SDDEV2 &> /dev/null
umount $SDDEV3 &> /dev/null

echo -n "SD formatting prepare clean boot partition  ..."
dd if=/dev/zero of=$SDDEV bs=512 count=1 &> /dev/null
if [[ $? != "0" ]];then
	echo "Unable to clean dos boot partition"
	exit 1
fi
echo "Done"

echo -n "SD formatting prepare partitions  ..."
RESULT=$(sudo sfdisk --force $SDDEV < $LAYOUT_FILE_TEMPLATE 2>&1)
if [[ $? != "0" ]];then
	echo "Unable to format disk"
	exit 1
fi
echo "Done"

echo -n "SD formatting prepare boot fs  ..."
RESULT=$(sudo mkdosfs -F16 -v -n "kernel" $SDDEV1 2>&1)
if [[ $? != "0" ]];then
	echo "Unable to make boot filesystem"
	exit 1
fi
echo "Done"

echo -n "SD formatting prepare root fs  ..."
RESULT=$(sudo mkfs.ext4 -j -O extent -L "rootfs"  $SDDEV2 2>&1)
if [[ $? != "0" ]];then
	echo "Unable to make root filesystem"
	exit 1
fi
echo "Done"

echo -n "SD formatting prepare data fs  ..."
RESULT=$(sudo mkdosfs -F32 -v -n "data" 	    	$SDDEV3 2>&1)
if [[ $? != "0" ]];then
	echo "Unable to make data filesystem"
	exit 1
fi
echo "Done"

echo -n "SD formatting prepare swap fs  ..."
RESULT=$(sudo mkswap $SDDEV4 2>&1)
if [[ $? != "0" ]];then
	echo "Unable to make swap partition"
	exit 1
fi
echo "Done"

exit 0
