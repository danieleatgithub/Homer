#!/bin/bash
echo 2 > /sys/class/gpio/export
echo "out" > /sys/class/gpio/pioA2/direction
echo 0  > /sys/class/gpio/pioA2/value
sleep 0.1
echo 1  > /sys/class/gpio/pioA2/value


