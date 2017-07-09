#/wks/linux-4.4.16/arch/arm/boot/dts/at91-sama5d3_acquaa5.dts
MODULES := issue

ARCH := arm
CROSS_COMPILE := arm-buildroot-linux-gnueabi-
obj-m := $(MODULES)
BUILDROOT = /wks/buildroot-2016.11
ROOT_OVERLAY = /wks/workspace/Homer/root_overlay

MAKEARCH := $(MAKE) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE)

homer:
	$(MAKE) -C $(ROOT_OVERLAY) homer
margie:
	$(MAKE) -C $(ROOT_OVERLAY) margie
all: homer
	