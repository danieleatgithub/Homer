#!/bin/bash
modprobe ina2xx

DEV="/sys/class/i2c-adapter/i2c-0/0-0040"


# curr1_input :: current through Rsens
I=$(cat $DEV/curr1_input)
I_RSENSE=$(echo "scale=6;$I/1000" | bc)

# in0_input :: voltage across Rsens
V0=$(cat $DEV/in0_input)
V_RSENSE=$(echo "scale=6;$V/1000" | bc)

# in1_input :: voltage from Bus to Gnd
V1=$(cat $DEV/in1_input)
V_BUS=$(echo "scale=6;$V/1000" | bc)

# power1_input:: power through Rsens
P=$(cat $DEV/power1_input)
P_RSENSE=$(echo "scale=6;$P/1000" | bc)

echo "Rsens: $P_RSENSE mW - $V_RSENSE mV $I_RSENS mA (Bus Voltage $V_BUS mV) 

#LOCALPA=$(echo "scale=2;$ALTITUDE/100*12" | bc)
#P=$(cat $DEV/pressure0_input)
#PRESSURE=$(echo "scale=2;$P/100+$LOCALPA" | bc)
#echo "Temp=$TEMP Pressure=$PRESSURE"

