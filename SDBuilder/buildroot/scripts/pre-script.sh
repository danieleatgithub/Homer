#!/bin/bash
#######################################################
#
# Buildroot pre script
#
# TARGET_DIR --from buildroot
#######################################################

# Executed in buildroot enviroment (Full Path)
. /wks/workspace/Homer/SDBuilder/enviroment.sh

echo " ******************* START PRE SCRIPT ***************"
#Setting GLOBALS
ROOT="/wks"
WORKSPACE="$WKS/workspace"

DST="$TARGET_DIR/configs"

AT91_ROOT="$WORKSPACE/at91bootstrap"
TARGET_HOMER="$WORKSPACE/Homer/SDBuilder/target"
GLIB_BUILD="/wks/${BUILDROOT_VER}/output/build/${GLIBC_VER}/build/elf"

#
# Modify skeleton
#

rm -Rf 	 $TARGET_DIR/var
mkdir -p $TARGET_DIR/var/cache
mkdir -p $TARGET_DIR/var/lock
mkdir -p $TARGET_DIR/var/log
mkdir -p $TARGET_DIR/var/pcmcia
mkdir -p $TARGET_DIR/var/run
mkdir -p $TARGET_DIR/var/spool
mkdir -p $TARGET_DIR/var/tmp
mkdir -p $TARGET_DIR/var/lib/dbus
mkdir -p $TARGET_DIR/var/empty
mkdir -p $TARGET_DIR/data
mkdir -p $TARGET_DIR/boot
mkdir -p $TARGET_DIR/configs
mkdir -p $TARGET_DIR/usr/local/bin
mkdir -p $TARGET_DIR/usr/local/lib
mkdir -p $TARGET_DIR/usr/local/etc
mkdir -p $TARGET_DIR/usr/local/share
mkdir -p $TARGET_DIR/usr/local/include
mkdir -p $TARGET_DIR/etc/org
cd $TARGET_DIR/var
ln -s ../run ./run
cd -
chmod 700 $TARGET_DIR/var/empty

#
# Install kernel modules
#
cd $LINUX_ROOT
rm -Rf $TARGET_DIR/lib/modules
make modules_install INSTALL_MOD_PATH=$TARGET_DIR ARCH=arm


#
# Save build configurations on target
#
cp $BR_ROOT/.config 							$DST/buildroot_.config
if [[ -e $BR_ROOT/output/build/${BBOX_VER}/.config ]]; then
	cp $BR_ROOT/output/build/${BBOX_VER}/.config $DST/buildroot_output_build_${BBOX_VER}_.config
fi
cp $LINUX_ROOT/.config 							$DST/linux_.config
cp $AT91_ROOT/.config 							$DST/at91bootstrap_.config
cp $LINUX_ROOT/arch/arm/boot/dts/acme-acqua.dtb 	$DST/.

#
# Copy user utility scripts
#
cp 			$TARGET_HOMER/usr/local/bin/* 	$TARGET_DIR/usr/local/bin/.
cp 			$TARGET_HOMER/root/* 			$TARGET_DIR/root/.
chmod 777 	$TARGET_DIR/usr/local/bin/*


#
# Setup DNS 
#
cat <<EOT > $TARGET_DIR/etc/resolv.conf
#
localhost 127.0.0.1
EOT

#
# Setup networking with vlans
#
#cp $TARGET_HOMER/usr/local/share/interfaces.template  $TARGET_DIR/usr/local/share/interfaces.template
cp $TARGET_DIR/etc/network/interfaces $TARGET_DIR/etc/network/interfaces.org
cp $TARGET_HOMER/etc/network/interfaces $TARGET_DIR/etc/network/interfaces
cp $TARGET_HOMER/etc/network/if-pre-up.d/vlan  		$TARGET_DIR/etc/network/if-pre-up.d/vlan && chmod 777 $TARGET_DIR/etc/network/if-pre-up.d/vlan
cp $TARGET_HOMER/etc/network/if-post-down.d/vlan 	$TARGET_DIR/etc/network/if-post-down.d/vlan && chmod 777 $TARGET_DIR/etc/network/if-post-down.d/vlan
echo "127.0.0.1	localhost" > $TARGET_DIR/etc/hosts
#echo "127.0.1.1	__NAME__" >> $TARGET_DIR/etc/hosts


#
# Setup FS
#
cat <<EOT1 > $TARGET_DIR/etc/fstab.add
/dev/mmcblk0p1	/boot  				vfat   		rw,defaults     0	1
/dev/mmcblk0p3	/data  				vfat   		rw,defaults     0	1
/dev/mmcblk0p4	none   				swap   		defaults        0	0
nodev 			/sys/kernel/debug	debugfs   	defaults   		0	0
EOT1


# 
# Setup Load modules
#
echo "8021q" 		>  $TARGET_DIR/etc/modules
echo "w1_gpio"		>> $TARGET_DIR/etc/modules
echo "w1_therm"		>> $TARGET_DIR/etc/modules


#
# SSHD
#
cp $TARGET_DIR/etc/ssh/sshd_config $TARGET_DIR/etc/ssh/sshd_config.org
sed -i 's/#PermitRootLogin prohibit-password/PermitRootLogin yes/g' $TARGET_DIR/etc/ssh/sshd_config

#
# BASH
#
cp $TARGET_HOMER/etc/profile $TARGET_DIR/etc/profile.my

#
# HTTPD
#
#cp $HOST_HOMER/lighttpd.conf 			$TARGET_DIR/etc/lighttpd/lighttpd.conf
cp $TARGET_HOMER/var/www/index.php 				$TARGET_DIR/var/www/index.php
cp $TARGET_HOMER/etc/lighttpd/modules.conf        $TARGET_DIR/etc/lighttpd/modules.conf
cp $TARGET_HOMER/etc/lighttpd/conf.d/fastcgi.conf 			$TARGET_DIR/etc/lighttpd/conf.d/fastcgi.conf
chmod 777 $TARGET_DIR/var/www/index.php

#
#  DHCPD
#
mkdir -p $TARGET_DIR/etc/dhcpd/
cp 		 $TARGET_HOMER/etc/dhcpd/dhcpd.conf 	$TARGET_DIR/etc/dhcp/dhcpd.conf
cp 		 $TARGET_DIR/etc/init.d/S80dhcp-server  $TARGET_DIR/etc/org/S80dhcp-server
sed -i 's/test -n "$INTERFACES" || exit 0//' $TARGET_DIR/etc/init.d/S80dhcp-server

#
# User Application
#
#cp $WORKSPACE/Homer/WinstarDisplayUtils/debug $TARGET_DIR/usr/local/bin
cp $TARGET_HOMER/usr/local/bin/* $TARGET_DIR/usr/local/bin/.
cp $TARGET_HOMER/usr/local/include/* $TARGET_DIR/usr/local/include/.
cp $TARGET_HOMER/usr/local/lib/* $TARGET_DIR/usr/local/lib/.
cp $TARGET_HOMER/usr/local/share/* $TARGET_DIR/usr/local/share/.
cp $TARGET_HOMER/etc/ld.so.conf $TARGET_DIR/etc/ld.so.conf


chmod 777 $TARGET_DIR/usr/local/bin/*
chmod 777 $TARGET_DIR/usr/local/lib/*

# Force ldconfig rerun and copy
#/sbin/ldconfig -r $TARGET_DIR -f $TARGET_DIR/etc/ld.so.conf
cp $GLIB_BUILD/ldconfig $TARGET_DIR/sbin/. && chmod 777 $TARGET_DIR/sbin/ldconfig
cp $GLIB_BUILD/ldd $TARGET_DIR/sbin/. && chmod 777 $TARGET_DIR/sbin/ldd
#cp $GLIB_BUILD/ldconfig $TARGET_DIR/root/. && chmod 777 $TARGET_DIR/root/ldconfig
#cp $GLIB_BUILD/ldd $TARGET_DIR/root/. && chmod 777 $TARGET_DIR/root/ldd


BUILD_DATE=$(date +"%Y%m%d%H%M%S")
echo "99.99.1-build${BUILD_DATE}" > $TARGET_DIR/etc/homer.version
echo " ******************* END PRE SCRIPT BUILD ${BUILD_DATE} ***************"
exit


