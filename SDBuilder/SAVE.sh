#!/bin/bash
#######################################################
#
# SAVE configurations
#
#######################################################
. ./enviroment.sh

BKP_ROOT="/wks/configs_backup"
BBOX=busybox-1.23.2
.  $BKP_ROOT/variables

BR_ROOT="/wks/buildroot-2015.08.1"
LX_ROOT="/wks/linux-3.16.1"
AT91_ROOT="/wks/git/at91bootstrap"


(( CURRENT +=1 ))

if [ $# -eq 1 ];then
	SAVE_NAME="$CURRENT-$1"
else
	SAVE_NAME=$CURRENT
fi



DST=$BKP_ROOT/$SAVE_NAME
mkdir $DST
cp $BR_ROOT/.config $DST/buildroot_.config
cp $BR_ROOT/output/build/$BBOX/.config $DST/buildroot_output_build_$BBOX_.config
cp $LX_ROOT/.config $DST/linux_.config
cp $AT91_ROOT/.config $DST/at91bootstrap_.config
cp $LX_ROOT/arch/arm/boot/dts/acme-acqua.dts $DST/.


cp $BR_ROOT/system/skeleton/etc/inittab $DST/skeleton_etc_inittab

echo '#!/bin/bash' >  $BKP_ROOT/variables
echo "CURRENT=$CURRENT" >> $BKP_ROOT/variables



