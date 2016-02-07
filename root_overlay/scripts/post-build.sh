#!/bin/bash
#######################################################
#
# Buildroot post build
#
# TARGET_DIR --from buildroot
#######################################################

echo " ******************* START PRE SCRIPT ***************"

# Executed in buildroot enviroment (Full Path)
. /wks/workspace/Homer/root_overlay/enviroment.sh


echo "***Install linux ..."
#
# Install kernel modules
#
cd $LINUX_ROOT
rm -Rf $TARGET_DIR/lib/modules
make modules_install INSTALL_MOD_PATH=$TARGET_DIR ARCH=arm
echo "***Linux installed"


#
# Setup FS
#
cat <<EOT1 > $TARGET_DIR/etc/fstab.add
/dev/mmcblk0p1	/boot  				vfat   		rw,defaults     0	1
/dev/mmcblk0p3	/data  				vfat   		rw,defaults     0	1
/dev/mmcblk0p4	none   				swap   		sw        0	0
nodev 			/sys/kernel/debug	debugfs   	defaults   		0	0
EOT1


chmod 700 $TARGET_DIR/var/empty
chmod 777 $TARGET_DIR/root/*

#
# SSHD Enable root on ssh
#
sed -i 's/#PermitRootLogin prohibit-password/PermitRootLogin yes/g' $TARGET_DIR/etc/ssh/sshd_config


BUILD_DATE=$(date +"%Y-%m-%d-%H%M%S")
echo "99.99.1-build${BUILD_DATE}" > $TARGET_DIR/etc/homer.version
echo " ******************* END PRE SCRIPT BUILD ${BUILD_DATE} ***************"
exit


