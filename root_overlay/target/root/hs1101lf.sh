#
# kernel module for hygrometer based on hs1101ls capacitor sensor (Frequency counter)
#
modprobe hs1101lf

DEV="/sys/bus/iio/devices/iio:device1"
if [[ ! -e ${DEV}/in_humidityrelative_raw ]]; then
        echo "device ${DEV}/in_humidityrelative_raw not exist"
        exit 1
fi
RH_RAW=$(cat ${DEV}/in_humidityrelative_raw)
RH=$(echo "scale=2;($RH_RAW/100)" | bc)
F=$(cat ${DEV}/frequency)
C=$(cat ${DEV}/cycles)
echo "RH=${RH}% F=${F}Hz Cycles=${C}"
