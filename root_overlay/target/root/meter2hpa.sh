#!/bin/bash
# utilities.sh
# Created on: 18/ago/2017
#     Author: daniele

I2CGET=/usr/sbin/i2cget
I2CDUMP=/usr/sbin/i2cdump
# http://www.bobhouse.it/Station/Root/Var_pressione.htm
# hPa each 5 meters altitude
MT5HPA=(\
  0      0.625  1.250  1.875  2.500  3.125  3.750  4.375  5.000  5.625  6.250  6.875  7.500  8.125  8.750  9.375 10.000 10.625 11.250 11.875 \
 12.500 13.125 13.750 14.375 15.000 15.625 16.250 16.875 17.500 18.125 18.750 19.375 20.000 20.625 21.250 21.875 22.500 23.125 23.750 24.375 \
 25.000 25.625 26.250 26.875 27.500 28.125 28.750 29.375 30.000 30.625 31.250 31.875 32.500 33.125 33.750 34.375 35.000 35.625 36.250 36.875 \
 37.500 38.125 38.750 39.375 40.000 40.625 41.250 41.875 42.500 43.125 43.750 44.375 45.000 45.625 46.250 46.875 47.500 48.125 48.750 49.375 \
 50.000 50.625 51.250 51.875 52.500 53.125 53.750 54.375 55.000 55.625 56.250 56.875 57.500 58.125 58.750 59.375 60.000 60.625 61.250 61.875 
 62.500 )
 
 
 
# Return hPa for a given altitude
# @param $1 altitude in meters
# @return hPa
function getHpa() {
	INDEX=$(echo "scale=1; $1/5" | bc)
	ROUNDED_INDEX=$(echo "scale=0; ($INDEX+0.5)/1" | bc)
	if [ $ROUNDED_INDEX -gt ${#MT5HPA[@]} ]; then 
		echo "ERROR getHpa: out of bound"
	else
		echo ${MT5HPA[$ROUNDED_INDEX]}
	fi
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
