/*******************************************************************************
 * Copyright (C) 2016  Daniele Colombo
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *******************************************************************************/

#define LOGDEVICE 	"homerd.device"
#define LOGHOMERD 	"homerd"
#define LOGSENSORS   "homerd.sensors"

// TODO get from properties
#define DAEMON_NAME "homerd"
#define ONEWIRE_0_PIN "PA01"
#define I2C_0_DAT_PIN "PA30"
#define I2C_0_CLK_PIN "PA31"
#define I2C_BUS "/dev/i2c-0"
#define SYSFS_GPIO_DIR "/sys/class/gpio"
#define LCD_BACKLIGHT_PIN "PA03"
#define LCD_RESET_PIN "PA02"
#define KEY_EVENT_DEVICE "/dev/input/event0"
#define SYSFS_ROOT "/sys"
#define LOCAL_ALTITUDE 330.0
