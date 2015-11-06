#!/bin/bash
#######################################################
#
# enviroment
#
#######################################################

WKS="/wks"
WORKSPACE="$WKS/workspace"
HOMER="$WORKSPACE/Homer"
SDROOT="$HOMERROOT/SDBuilder"
SDSCRIPTS="$SDROOT/SDscripts"
BR_ROOT="$WKS/buildroot-2015.08.1"
TARGET_HOMER="$SDROOT/target"

TARGET_ROOTFS_DIR=/media/rootfs
TARGET_KERNEL_DIR=/media/kernel
TARGET_DATA_DIR=/media/data
LINUX_ROOT="$WKS/linux-3.16.1"

SDFORMAT="$SDSCRIPTS/sdformat.sh"

AT91BINARY="$WORKSPACE/"at91bootstrap/binaries"

SDDEV="/dev/sdd"
SDDEV1="/dev/sdd1" # /boot (FAT16)
SDDEV2="/dev/sdd2" # /root (ext4)
SDDEV3="/dev/sdd3" # swap
SDDEV4="/dev/sdd4" # /data