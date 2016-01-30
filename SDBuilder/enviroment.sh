#!/bin/bash
#######################################################
#
# enviroment
#
#######################################################

BBOX_VER="none"
BUILDROOT_VER="buildroot-2015.11.1"
LINUX_VER="linux-3.16.1"
GLIBC_VER="glibc-2.21"


WKS="/wks"
WORKSPACE="$WKS/workspace"
HOMERROOT="$WORKSPACE/Homer"
SDROOT="$HOMERROOT/SDBuilder"
SDSCRIPTS="$SDROOT/SDscripts"
BR_ROOT="$WKS/$BUILDROOT_VER"
TARGET_HOMER="$SDROOT/target"

TARGET_ROOTFS_DIR="/media/rootfs"
TARGET_KERNEL_DIR="/media/kernel"
TARGET_DATA_DIR="/media/data"
LINUX_ROOT="$WKS/$LINUX_VER"

SDFORMAT="$SDSCRIPTS/sdformat.sh"

AT91_ROOT="$WORKSPACE/at91bootstrap"
AT91BINARY="$AT91_ROOT/binaries"
BKP_ROOT="$SDROOT/config_backups"

DEVICE="sdc"
SDDEV="/dev/${DEVICE}"
SDDEV1="/dev/${DEVICE}1" # /boot (FAT16)
SDDEV2="/dev/${DEVICE}2" # /root (ext4)
SDDEV3="/dev/${DEVICE}3" # swap
SDDEV4="/dev/${DEVICE}4" # /data
