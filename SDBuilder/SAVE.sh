#!/bin/bash
#######################################################
#
# SAVE configurations
#
#######################################################
. ./enviroment.sh
VARIABLES=$BKP_ROOT/variables.sh
.  $VARIABLES

(( CURRENT +=1 ))

if [ $# -eq 1 ];then
	SAVE_NAME="$CURRENT-$1"
else
	SAVE_NAME=$CURRENT
fi



DST=$BKP_ROOT/$SAVE_NAME
mkdir $DST
cp $BR_ROOT/.config $DST/buildroot_.config
if [[ -e $BR_ROOT/output/build/${BBOX_VER}/.config ]]; then
	cp $BR_ROOT/output/build/${BBOX_VER}/.config $DST/buildroot_output_build_${BBOX_VER}_.config
fi
cp $LINUX_ROOT/.config $DST/linux_.config
cp $AT91_ROOT/.config $DST/at91bootstrap_.config
#cp $LINUX_ROOT/arch/arm/boot/dts/acme-acqua.dts $DST/.
cp $LINUX_ROOT/arch/arm/boot/dts/at91-sama5d3_acquaa5.dts $DST/.

echo '#!/bin/bash' >  $VARIABLES
echo "CURRENT=$CURRENT" >> $VARIABLES



