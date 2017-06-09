#!/bin/bash
#######################################################
# MMC Formatter
#######################################################


# source build enviroment
SPATH=$(dirname $0)
. $SPATH/../enviroment.sh
. $SPATH/layouts.sh

if [ $# -ne 0 ];then
	echo "Usage: $(basename $0)"
	echo "Format a mmc in three partition according to a saved layout"
	exit 1
fi

# find layout
echo -n "SD formatting prepare layout ..." >&2
SF_SIZE=$(sfdisk -s $SDDEV)
layout_found=$(get_layout $SF_SIZE)
layout_template=${SDSCRIPTS}/"sfdisk.${layout_found}GB.layout"
if [ ! -f "${layout_template}" ];then
	echo "Layout error $SF_SIZE"
	echo ${layout_found}
	layouts_help
	exit 1
fi
echo -n " ${layout_found} GB " >&2
layout_work_file="/tmp/layout"
cp $layout_template $layout_work_file
sed -i "s/__DEVICE__/${DEVICE}/g" $layout_work_file
echo "Done" >&2

echo -n "SD formatting umount partitions ..." >&2
# unmount target devices ignore errors
umount $SDDEV1 &> /dev/null 
umount $SDDEV2 &> /dev/null 
umount $SDDEV3 &> /dev/null
echo "Done" >&2

echo -n "SD formatting clean boot partition  ..." >&2
dd if=/dev/zero of=$SDDEV bs=512 count=1 &>> $LOG
if [[ $? != "0" ]];then
	echo "Unable to clean dos boot partition"
	exit 1
fi
echo "Done" >&2

echo -n "SD formatting create partition ..." >&2
RESULT=$(sudo sfdisk --force $SDDEV < $layout_template 2>&1)
if [[ $? != "0" ]];then
	echo "Unable to partition sd $SDDEV"
	echo $RESULT >> $LOG
	exit 1
fi
echo "Done" >&2

echo -n "SD formatting mkdosfs boot fs  ..." >&2
RESULT=$(sudo mkdosfs -F16 -v -n "kernel" $SDDEV1 2>&1)
if [[ $? != "0" ]];then
	echo "Unable to make boot filesystem"
	echo $RESULT >> $LOG
	exit 1
fi
echo "Done" >&2

echo -n "SD formatting mkfs.ext4 root fs  ..." >&2
RESULT=$(sudo mkfs.ext4 -j -O extent -L "rootfs"  $SDDEV2 2>&1)
if [[ $? != "0" ]];then
	echo "Unable to make root filesystem"
	echo $RESULT > $LOG
	exit 1
fi
echo "Done" >&2

echo -n "SD formatting mkdosfs data fs  ..." >&2
RESULT=$(sudo mkdosfs -F32 -v -n "data" 	    	$SDDEV3 2>&1)
if [[ $? != "0" ]];then
	echo "Unable to make data filesystem"
	echo $RESULT >> $LOG
	exit 1
fi
echo "Done" >&2

echo -n "SD formatting make swap fs  ..." >&2
RESULT=$(sudo mkswap $SDDEV4 2>&1)
if [[ $? != "0" ]];then
	echo "Unable to make swap partition"
	echo $RESULT >> $LOG
	exit 1
fi
echo "Done" >&2
echo "SD Ready" >&2
exit 0
