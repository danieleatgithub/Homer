#!/bin/bash
#######################################################
#
# RESTORE configurations
#
#######################################################
. ./enviroment.sh

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

# Save current for undo
DST=$BKP_ROOT/undo
rm -Rf $DST
mkdir -p $DST
cp $BR_ROOT/.config $DST/buildroot_.config
if [[ -e $BR_ROOT/output/build/${BBOX_VER}/.config ]]; then
	cp $BR_ROOT/output/build/${BBOX_VER}/.config $DST/buildroot_output_build_${BBOX_VER}_.config
fi
cp $LINUX_ROOT/.config $DST/linux_.config
cp $AT91_ROOT/.config $DST/at91bootstrap_.config
cp $LINUX_ROOT/arch/arm/boot/dts/acme-acqua.dts $DST/.

# echo command
echo "cp Commands for cut & paste"
echo "---------------------------"
echo "cp $SRC/buildroot_.config $BR_ROOT/.config"
if [[ -e $BR_ROOT/output/build/${BBOX_VER}/.config ]]; then
echo "cp $SRC/buildroot_output_build_${BBOX_VER}_.config $BR_ROOT/output/build/${BBOX_VER}/.config"
fi
echo "cp $SRC/linux_.config $LINUX_ROOT/.config"
echo "cp $SRC/at91bootstrap_.config $AT91_ROOT/.config"
echo "cp $SRC/acme-acqua.dts $LINUX_ROOT/arch/arm/boot/dts/acme-acqua.dts"
