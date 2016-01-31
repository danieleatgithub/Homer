#!/bin/bash
echo 0  > /sys/class/gpio/pioA2/value
sleep 0.1
echo 1  > /sys/class/gpio/pioA2/value


