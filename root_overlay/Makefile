#/wks/linux-4.4.16/arch/arm/boot/dts/at91-sama5d3_acquaa5.dts
MODULES := issue

ARCH := arm
CROSS_COMPILE := arm-buildroot-linux-gnueabi-
obj-m := $(MODULES)
BUILDROOT = /wks/buildroot-2016.11
TARGET_ROOT = $(shell pwd)/target

MAKEARCH := $(MAKE) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE)

homer:
	echo "homer" >  $(TARGET_ROOT)/etc/hostname
	chmod +x $(TARGET_ROOT)/etc/init.d/S98homerd
	$(MAKE) -C $(BUILDROOT)
margie:
	echo "margie" >  $(TARGET_ROOT)/etc/hostname
	chmod -x $(TARGET_ROOT)/etc/init.d/S98homerd
	$(MAKE) -C $(BUILDROOT)
all: homer
	