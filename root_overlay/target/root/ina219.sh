#!/bin/bash
modprobe ina2xx

DEV="/sys/class/i2c-adapter/i2c-0/0-0040/hwmon/hwmon0"

# shunt resistor in micro-ohm
# 0.1 ohm (but in dts is is configured 10000 micro ohms
# in order to get a more precise value
RSENSE=$(cat $DEV/shunt_resistor)

# curr1_input :: current through Rsens
I=$(cat $DEV/curr1_input)
I_RSENSE=$(echo "scale=2;$I/10" | bc)

# in0_input :: voltage across Rsens
V0=$(cat $DEV/in0_input)
V_RSENSE=$(echo "scale=2;$V0/10" | bc)

# in1_input :: voltage from Bus to Gnd
V1=$(cat $DEV/in1_input)
V_BUS=$(echo "scale=2;$V1/1000" | bc)

# power1_input:: power through Rsens
P=$(cat $DEV/power1_input)
P_RSENSE=$(echo "scale=2;$P/1000000" | bc)

#echo "Rsens: $P_RSENSE mW - $V_RSENSE mV $I_RSENSE mA (Bus Voltage $V_BUS V) RSENSE $RSENSE"
echo "Current energy usage: $P_RSENSE mW $I_RSENSE mA $V_BUS V"
