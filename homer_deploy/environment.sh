#!/bin/bash
#######################################################
#
# enviroment
#
#######################################################

BUILDROOT_VER="buildroot-2018.02.9"
LINUX_VER="4.9.118"
DEVTREE="at91-sama5d3_acquaa5"
TARGET_DTB="/boot/at91-sama5d3_acqua.dtb"
TARGET_DOCS="/data/docs"

ARCH=arm
CROSS_COMPILE=arm-buildroot-linux-gnueabi-
WORKSPACE=/wks/workspace
HS1101LF=${WORKSPACE}/KernelModules/hs1101lf
LINUX_DTS=${WORKSPACE}/Homer/homer_deploy
BUILDROOT=/wks/${BUILDROOT_VER}
ROOT_OVERLAY=${WORKSPACE}/Homer/root_overlay
SD_BUILDER=${WORKSPACE}/Homer/SDBuilder
TARGET_HOMER=${ROOT_OVERLAY}/target
AT91_ROOT=${WORKSPACE}/at91bootstrap

