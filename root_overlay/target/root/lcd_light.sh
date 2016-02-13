#!/bin/bash
echo 3 > /sys/class/gpio/export
echo "out" > /sys/class/gpio/pioA3/direction
echo $1  > /sys/class/gpio/pioA3/value

