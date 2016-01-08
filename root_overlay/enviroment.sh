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
LINUX_ROOT="$WKS/$LINUX_VER"
TARGET_HOMER="$HOMERROOT/root_overlay/target"