#!/bin/bash
#######################################################
#
# RESTORE configurations
#
#######################################################

SCRIPT_PATH=$(dirname ${BASH_SOURCE[0]})
. /wks/workspace/Homer/homer_deploy/environment.sh

if [ $# -ne 1 ];then
	echo "Usage: $(basename $0) <index to restore>"
	exit 1
fi
SRC=${SCRIPT_PATH}/config_backups/$1
if [ ! -d $SRC ];then
	echo "$SRC is not a folder"
	exit 1
fi

.  ${SCRIPT_PATH}/config_backups/variables.sh

# Save current for undo
${SCRIPT_PATH}/SAVE.sh undo

# echo command
echo "cp Commands for cut & paste"
echo "---------------------------"
echo "cp $SRC/buildroot_.config $BR_ROOT/.config"
if [[ -e $BR_ROOT/output/build/${BBOX_VER}/.config ]]; then
	echo "cp $SRC/buildroot_output_build_${BBOX_VER}_.config $BR_ROOT/output/build/${BBOX_VER}/.config"
fi
echo "cp $SRC/linux_.config   ${BUILDROOT}/output/build/linux-${LINUX_VER}/.config"
echo "cp $SRC/linux_.config_p ${BUILDROOT}/patch/linux_.config"

if [[ -e $SRC/at91bootstrap_.config ]]; then
	echo "cp $SRC/at91bootstrap_.config $AT91_ROOT/.config"
fi
echo "cp $SRC/${DEVTREE}.dts ${LINUX_DTS}/${DEVTREE}.dts"
