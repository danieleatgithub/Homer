
include /wks/workspace/Homer/homer_deploy/environment.sh	


# Config and dts backups 	
_backup: 
	$(SD_BUILDER)/SAVE.sh
	
# Rebuild kernel and device tree
_linux:
	$(MAKE) -C $(BUILDROOT) linux-rebuild

_linux-xconfig:
	$(MAKE) -C $(BUILDROOT) linux-xconfig

_buildroot-xconfig:
	$(MAKE) -C $(BUILDROOT) xconfig

# Prepare rootfs, buildroot and burn SD for production 
homer:
	$(MAKE) -C $(HS1101LF)
	$(MAKE) -C $(ROOT_OVERLAY) homer
	$(MAKE) -C $(BUILDROOT)
	
# Prepare rootfs, buildroot and burn SD for tests 
margie:
	$(MAKE) -C $(HS1101LF)
	$(MAKE) -C $(ROOT_OVERLAY) margie
	$(MAKE) -C $(BUILDROOT)
	
# Prepare  burn SD 
burn: 
	$(SD_BUILDER)/BURN.sh
	
	


#
# Copy DTB on target
# 
try_dtb_margie:
	$(shell pwd)/CP_DTB margie
try_dtb_homer:
	$(shell pwd)/CP_DTB homer

# default target burn SD for production
all: burn_homer


	