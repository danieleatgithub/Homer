#!/bin/bash
#######################################################
#
# enviroment
#
#######################################################


BBOX_VER="none"


BUILDROOT_VER="buildroot-2016.11"


LINUX_VER="linux-4.4.16"

GLIBC_VER="glibc-2.21"
DTB="at91-sama5d3_acquaa5.dtb"
#DTB="acme-acqua.dtb"

WKS="/wks"
WORKSPACE="$WKS/workspace"
HOMERROOT="$WORKSPACE/Homer"
LINUX_ROOT="$WKS/$LINUX_VER"
TARGET_HOMER="$HOMERROOT/root_overlay/target"
