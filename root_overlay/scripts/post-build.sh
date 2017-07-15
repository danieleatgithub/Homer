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

#
# Setting SD Version
#
. /wks/workspace/Homer/SDBuilder/config_backups/variables.sh
HOMER_MAJ=99
HOMER_MIN=99
HOMER_REV=$CURRENT

echo "***Install linux ..."
#
# Install kernel modules
#
cd $LINUX_ROOT
rm -Rf $TARGET_DIR/lib/modules
make modules_install INSTALL_MOD_PATH=$TARGET_DIR ARCH=arm
echo "***Linux installed"
cd $HS110LF
make modules_install INSTALL_MOD_PATH=$TARGET_DIR ARCH=arm
echo "***Custom modules installed"
cp $LINUX_ROOT/tools/iio/iio_event_monitor 	$TARGET_DIR/sbin/.
cp $LINUX_ROOT/tools/iio/lsiio 				$TARGET_DIR/sbin/.
cp $LINUX_ROOT/tools/iio/generic_buffer 	$TARGET_DIR/sbin/.
echo "***Kernel tools installed"

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
echo "${HOMER_MAJ}.${HOMER_MIN}.${HOMER_REV}-build_${BUILD_DATE}" > $TARGET_DIR/etc/homer.version
echo " ******************* END PRE SCRIPT BUILD ${BUILD_DATE} ***************"
exit


