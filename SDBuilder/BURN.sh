#!/bin/bash
#######################################################
#
# SD burner
#
#######################################################
. ./enviroment.sh
 
if [ -z ${SUDO+x} ]; then
	echo "BURN: password for sudo is unset"
	exit 1
fi

export LOG="/var/www/html/logs/burn.log"
echo "BURN: logs in http://debian-devel/logs/burn.log"

date > $LOG

rm -Rf /wks/data/logs &>> $LOG 
mkdir  /wks/data/logs &>> $LOG 
ping -c 1 -W 1 192.168.1.200 &>> $LOG 
if (( $? == 0 )); then
	echo "BURN: Copy data from live homer" >&2
	scp -p root@192.168.1.200:/data/logs/* /wks/data/logs &>> $LOG
else
	echo "offline burning" >&2
fi

echo "Start Burning Homer" >> $LOG
echo $SUDO | sudo -S -E $SDSCRIPTS/makefs.sh Homer

echo "BURN: copy data in sd" >&2
echo $SUDO | sudo -S -E mount $SDDEV3 $TARGET_DATA_DIR
if [[ $? !=  "0" ]];then
	echo "BURN: Failed to mount data partition" >&2
	exit 1
fi
ls -l $TARGET_DATA_DIR >&2
echo $SUDO | sudo -S -E mkdir -p $TARGET_DATA_DIR/logs 
echo $SUDO | sudo -S -E chmod -R 777 $TARGET_DATA_DIR/logs
echo $SUDO | sudo -S -E cp /wks/data/logs/* $TARGET_DATA_DIR/logs/.
echo "BURN: Logs copied" >&2
ls -l $TARGET_DATA_DIR/logs >&2
echo $SUDO | sudo -S -E umount $TARGET_DATA_DIR


echo "____________________________________" >&2
echo "                                    " >&2
echo "You can remove SD card ... Good Luck" >&2
echo "____________________________________" >&2
	