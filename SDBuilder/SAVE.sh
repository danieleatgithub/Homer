#!/bin/bash
#######################################################
#
# SAVE configurations
#
#######################################################

SCRIPT_PATH=$(dirname ${BASH_SOURCE[0]})
. /wks/workspace/Homer/homer_deploy/environment.sh
. ${SCRIPT_PATH}/devices.sh

if [ -z ${BUILDROOT} ]; then
	echo "${BUILDROOT} not exist"
	exit
fi


VARIABLES=${SCRIPT_PATH}/config_backups/variables.sh
.  $VARIABLES

(( CURRENT +=1 ))

if [ $# -eq 1 ];then
	SAVE_NAME="$CURRENT-$1"
else
	SAVE_NAME=$CURRENT
fi



DST=${SCRIPT_PATH}/config_backups/${SAVE_NAME}
mkdir $DST

cp ${BUILDROOT}/.config $DST/buildroot_.config

if [[ -e ${BUILDROOT}/output/build/${BBOX_VER}/.config ]]; then
	cp $BUILDROOT/output/build/${BBOX_VER}/.config $DST/buildroot_output_build_${BBOX_VER}_.config
fi

if [[ -e $AT91_ROOT/.config ]]; then
	cp $AT91_ROOT/.config $DST/at91bootstrap_.config
fi

cp ${BUILDROOT}/output/build/linux-${LINUX_VER}/.config $DST/linux_.config
cp ${BUILDROOT}/patch/linux_.config $DST/linux_.config_p

cp ${LINUX_DTS}/${DEVTREE}.dts $DST/.

echo '#!/bin/bash' >  $VARIABLES
echo "CURRENT=$CURRENT" >> $VARIABLES

echo "Backup Done in $DST"
exit 0


