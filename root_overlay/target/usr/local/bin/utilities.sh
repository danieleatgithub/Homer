#!/bin/bash
# utilities.sh
# Created on: 18/ago/2017
#     Author: daniele

I2CGET=/usr/sbin/i2cget
I2CDUMP=/usr/sbin/i2cdump

# Return hPa for a given altitude
# @param $1 altitude in meters
# @return hPa
function getHpa() {
	ALTITUDE=$1
	HPA=$(echo "scale=4;$ALTITUDE/100*12.5" | bc)
	echo $HPA
}


# https://www.acmesystems.it/acqua_at24mac402
# read serial number from chip in i2c-3 0x58
# @return serial number 0A700800641008417076A000A0000100
function getSerial() {
	ACQUA_EEPROM=$($I2CDUMP -y -r 0x80-0x8f 3 0x58 2>&1 | tail -n 1)
	#80: 0a 70 08 00 64 10 08 41 70 76 a0 00 a0 00 01 00 ?p?.d??Apv?.?.?.
	SERIAL=$(echo ${ACQUA_EEPROM:4:48} | tr '[:lower:]' '[:upper:]' | tr -d ' ' )
	if [[ ${#SERIAL} -ne 32 ]];then
		echo 'ERROR getSerial: 00000000000000000000000000000000'
		exit 1
	else
		echo $SERIAL
		exit 0
	fi
}


# https://www.acmesystems.it/acqua_at24mac402
# read mac address number from chip in i2c-3 0x58
# @return mac address fc:c2:3d:0a:c8:a6
function getMac() {
	ACQUA_EEPROM=$($I2CDUMP -y -r 0x9a-0x9f 3 0x58 2>&1 | tail -n 1 )
	MAC=$(echo ${ACQUA_EEPROM:30:24} | tr ' ' ':')
	if [ ${#MAC} -ne 17 ] || [ "$MAC" == "XX:XX:XX:XX:XX:XX" ];then
		echo 'ff:ff:ff:ff:ff:ff'
		exit 1
	else
		echo $MAC
		exit 0
	fi
}
