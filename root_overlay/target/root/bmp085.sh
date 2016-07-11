#!/bin/bash
modprobe bmp085_i2c

# Local pressure is 12 mbar each 100 mt altitude
# ----------------------------------------------
ALTITUDE=305
# ----------------------------------------------

DEV="/sys/class/i2c-adapter/i2c-0/0-0077"
T=$(cat $DEV/temp0_input)
TEMP=$(echo "scale=2;$T/10" | bc)

LOCALPA=$(echo "scale=2;$ALTITUDE/100*12" | bc)
P=$(cat $DEV/pressure0_input)
PRESSURE=$(echo "scale=2;$P/100+$LOCALPA" | bc)
echo "Temp=$TEMP Pressure=$PRESSURE"