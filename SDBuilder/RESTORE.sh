#!/bin/bash
# 
#
. ./enviroment.sh

BKP_ROOT="/home/daniele/configs_backup"

if [ $# -ne 1 ];then
	echo "Usage: $(basename $0) <index to restore>"
	exit 1
fi
SRC=$BKP_ROOT/$1
if [ ! -d $SRC ];then
	echo "$SRC is not a folder"
	exit 1
fi

.  $BKP_ROOT/variables
BR_ROOT="/home/daniele/buildroot-2014.11"
LX_ROOT="/home/daniele/linux-3.16.1"
AT91_ROOT="/home/daniele/git/at91bootstrap"

# Save current for undo
DST=$BKP_ROOT/undo
rm -Rf $DST
mkdir -p $DST
cp $BR_ROOT/.config $DST/buildroot_.config
cp $BR_ROOT/output/build/busybox-1.22.1/.config $DST/buildroot_output_build_busybox-1.22.1_.config
cp $LX_ROOT/.config $DST/linux_.config
cp $AT91_ROOT/.config $DST/at91bootstrap_.config
cp $LX_ROOT/arch/arm/boot/dts/acme-acqua.dts $DST/.

# echo command
echo "cp Commands for cut & paste"
echo "---------------------------"
echo "cp $SRC/buildroot_.config $BR_ROOT/.config"
echo "cp $SRC/buildroot_output_build_busybox-1.22.1_.config $BR_ROOT/output/build/busybox-1.22.1/.config"
echo "cp $SRC/linux_.config $LX_ROOT/.config"
echo "cp $SRC/at91bootstrap_.config $AT91_ROOT/.config"
echo "cp $SRC/acme-acqua.dts $LX_ROOT/arch/arm/boot/dts/acme-acqua.dts"
