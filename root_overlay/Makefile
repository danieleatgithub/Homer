# 
# Reconfigure target fs to test or production board
# Run before make buildroot
#

include /wks/workspace/Homer/homer_deploy/environment.sh	

homer:
	echo "homer" >  $(TARGET_HOMER)/etc/hostname
	chmod +x $(TARGET_HOMER)/etc/init.d/S98homerd

margie:
	echo "margie" >  $(TARGET_HOMER)/etc/hostname
	chmod -x $(TARGET_HOMER)/etc/init.d/S98homerd

all: homer
	