#!/bin/bash
modprobe bmp280_i2c
modprobe bmp280

# Local pressure is 12 mbar each 100 mt altitude
# ----------------------------------------------
ALTITUDE=330
# ----------------------------------------------
DEV="/sys/class/i2c-dev/i2c-0/device/0-0077/iio:device0"
if [[ ! -e ${DEV}/in_pressure_input ]]; then
	echo "device ${DEV}/in_pressure_input not exist"
	exit 1
fi

T=$(cat $DEV/in_temp_input)
TEMP=$(echo "scale=2;$T/1000" | bc)

LOCALPA=$(echo "scale=2;$ALTITUDE/100*12.5" | bc)
P=$(cat $DEV/in_pressure_input)
PRESSURE=$(echo "scale=2;($P*10)+$LOCALPA" | bc)
echo "Temp=$TEMP Pressure=$PRESSURE"