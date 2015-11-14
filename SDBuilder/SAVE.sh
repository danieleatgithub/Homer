#!/bin/bash
#######################################################
#
# SAVE configurations
#
#######################################################
. ./enviroment.sh

.  $BKP_ROOT/variables

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
cp $LINUX_ROOT/.config $DST/linux_.config
cp $AT91_ROOT/.config $DST/at91bootstrap_.config
cp $LINUX_ROOT/arch/arm/boot/dts/acme-acqua.dts $DST/.

echo '#!/bin/bash' >  $BKP_ROOT/variables
echo "CURRENT=$CURRENT" >> $BKP_ROOT/variables



