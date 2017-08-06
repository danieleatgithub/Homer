
include /wks/workspace/Homer/homer_deploy/environment.sh	
	
# Rebuild kernel and device tree
linux:
	$(MAKE) -C $(BUILDROOT) linux-rebuild

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
	
# Prepare rootfs, buildroot and burn SD for production 
burn_homer: 
	$(MAKE) -C $(SD_BUILDER)   homer	
	
# Prepare rootfs, buildroot and burn SD for tests 
burn_margie: 
	$(MAKE) -C $(SD_BUILDER)  margie	
	
# Config and dts backups 	
backup: 
	$(SD_BUILDER)/SAVE.sh

#
# Copy DTB on target
# 
debug_margie:
	$(shell pwd)/CP_DTB margie
debug_homer:
	$(shell pwd)/CP_DTB homer

# default target burn SD for production
all: burn_homer


	