#!/bin/bash
#######################################################
#
# SDD devices
#
#######################################################


DEVICE="sdd"
SDDEV="/dev/${DEVICE}"
SDDEV1="/dev/${DEVICE}1" # /boot (FAT16)
SDDEV2="/dev/${DEVICE}2" # /root (ext4)
SDDEV3="/dev/${DEVICE}3" # swap
SDDEV4="/dev/${DEVICE}4" # /data
