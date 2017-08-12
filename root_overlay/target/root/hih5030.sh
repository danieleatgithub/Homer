modprobe at91_adc

DEV="/sys/bus/iio/devices/iio:device0"
if [[ ! -e ${DEV}/in_voltage0_raw ]]; then
        echo "device ${DEV}/in_voltage0_raw not exist"
        exit 1
fi
V=$(cat ${DEV}/in_voltage0_raw)
S=$(cat ${DEV}/in_voltage_scale)
VOLT=$(echo "scale=2;($V*$S)/1000" | bc)
RH=$(echo "scale=2;((($VOLT/3.0)-0.16)/0.0062)" | bc)
echo "RH=${RH}% VOLT=${VOLT}V SCALE=${S}"

