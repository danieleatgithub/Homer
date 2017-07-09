MODULES := x

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
	