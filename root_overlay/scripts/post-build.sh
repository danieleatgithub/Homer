#!/bin/bash
#######################################################
#
# Buildroot post build
#
# TARGET_DIR --from buildroot
#######################################################

echo " ******************* START PRE SCRIPT ***************"

# Executed in buildroot enviroment (Full Path)
. /wks/workspace/Homer/homer_deploy/environment.sh

#
# Setting SD Version
#
. /wks/workspace/Homer/SDBuilder/config_backups/variables.sh
HOMER_MAJ=99
HOMER_MIN=99
HOMER_REV=$CURRENT

#
# Setup FS
# FIXME: buildroot overwrited
cat <<EOT1 > ${TARGET_DIR}/etc/fstab
# <file system> <mount pt>      <type>  <options>       <dump>  <pass>
/dev/root       /               ext2    rw,noauto       0       1
proc            /proc           proc    defaults        0       0
devpts          /dev/pts        devpts  defaults,gid=5,mode=620,ptmxmode=0666   0       0
tmpfs           /dev/shm        tmpfs   mode=0777       0       0
tmpfs           /tmp            tmpfs   mode=1777       0       0
tmpfs           /run            tmpfs   mode=0755,nosuid,nodev  0       0
sysfs           /sys            sysfs   defaults        0       0
/dev/mmcblk0p1	/boot  				vfat   		rw,defaults     0	1
/dev/mmcblk0p3	/data  				vfat   		rw,defaults     0	1
/dev/mmcblk0p4	none   				swap   		sw        0	0
nodev 			/sys/kernel/debug	debugfs   	defaults   		0	0
EOT1


chmod 700 ${TARGET_DIR}/var/empty
chmod 777 ${TARGET_DIR}/root/*
cp /wks/workspace/Homer/homerd/homerd/homerd $TARGET_DIR/bin/.

make -C ${HS1101LF} modules_install
#
# SSHD Enable root on ssh
#
sed -i 's/#PermitRootLogin prohibit-password/PermitRootLogin yes/g' $TARGET_DIR/etc/ssh/sshd_config


BUILD_DATE=$(date +"%Y-%m-%d-%H%M%S")
echo "${HOMER_MAJ}.${HOMER_MIN}.${HOMER_REV}-build_${BUILD_DATE}" > $TARGET_DIR/etc/homer.version
echo " ******************* END PRE SCRIPT BUILD ${BUILD_DATE} ***************"
exit


