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

homer:
	$(MAKE) -C $(HS1101LF) 
	$(MAKE) -C $(LINUX-DTS)
	$(MAKE) -C $(ROOT_OVERLAY) homer
	
margie:
	$(MAKE) -C $(HS1101LF) 
	$(MAKE) -C $(LINUX-DTS)
	$(MAKE) -C $(ROOT_OVERLAY) margie
	
burn_margie:
	$(MAKE) -C $(SD_BUILDER)  margie
all: homer


	