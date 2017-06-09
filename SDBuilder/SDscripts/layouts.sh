#!/bin/bash
#######################################################
# MMC Formatter
#######################################################

declare -A layouts
layouts[7761920]=8
layouts[7838720]=8   #  8GB Samsung
layouts[3866624]=4   #  4GB 
layouts[15558144]=16 # 16GB 

# -----------------------------------------------------
function get_layout {
	geometry=$1
	if ! [ ${layouts[$geometry]+abcde} ];then
		echo "LAYOUT ERROR Unknow geometry $geometry"
		return
	fi
	echo ${layouts[$geometry]}
}

function layouts_help  {
	
	cat << EOH
==================== HELP ============
Format SD Card 

Disk identifier: 0xc4814c7a
Device     Boot   Start     End Sectors  Size Id Type
/dev/sdd1          2048 1021952 1019905  498M  6 FAT16
/dev/sdd2       1024000 3121151 2097152    1G 83 Linux
/dev/sdd3       3121152 7315455 4194304    2G  c W95 FAT32 (LBA)
/dev/sdd4       7315456 7733247  417792  204M 82 Linux swap / Solaris

Format according to example
#sfdisk /dev/sdd
Save partition layout
#sfdisk -d /dev/sdd > sfdisk.8GB.layout
Get geometry
#sfdisk -s /dev/sdd
15558144
Add new layout to layouts.sh
layouts[15558144]=16 # 16GB 
=======================================
EOH
	
}
