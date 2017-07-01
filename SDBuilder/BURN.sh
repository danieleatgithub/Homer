#!/bin/bash
#######################################################
#
# SD burner
#
#######################################################
. ./enviroment.sh
 
export LOG="/var/www/html/logs/burn.log"
echo "logs in http://debian-devel/logs/burn.log"
echo "Start Burning Homer" > $LOG
echo $SUDO | sudo -S -E $SDSCRIPTS/makefs.sh Homer

	
