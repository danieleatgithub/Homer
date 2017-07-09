MODULES := x

ARCH := arm
CROSS_COMPILE := arm-buildroot-linux-gnueabi-
obj-m := $(MODULES)

HS1101LF = /wks/workspace/KernelModules/hs1101lf
LINUX-DTS = /wks/workspace/KernelModules/linux-dts
BUILDROOT = /wks/buildroot-2016.11
ROOT_OVERLAY = /wks/workspace/Homer/root_overlay
SD_BUILDER = /wks/workspace/Homer/SDBuilder/

MAKEARCH := $(MAKE) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE)
	
margie:
	$(SD_BUILDER)/BURN.sh margie
		



	