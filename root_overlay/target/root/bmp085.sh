#!/bin/bash
DEV="/sys/class/i2c-adapter/i2c-0/0-0077"
T=$(cat $DEV/temp0_input)
TEMP=$(echo "scale=1;$T/10" | bc)
P=$(cat $DEV/pressure0_input)
PRESSURE=$(echo "scale=1;$P/100" | bc)
echo "Temp=$TEMP Pressure=$PRESSURE"