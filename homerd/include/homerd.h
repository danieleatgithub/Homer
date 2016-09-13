/*
 * homerd.h
 *
 *  Created on: 08/feb/2016
 *      Author: daniele
 */

#define LOGDEVICE 	"homerd.device"
#define LOGHOMERD 	"homerd"


// FIXME get from properties
#define DAEMON_NAME "homerd"
#define ONEWIRE_0_PIN "PA01"
#define I2C_0_DAT_PIN "PA30"
#define I2C_0_CLK_PIN "PA31"
#define I2C_BUS "/dev/i2c-0"
#define SYSFS_GPIO_DIR "/sys/class/gpio"
#define LCD_BACKLIGHT_PIN "PA03"
#define LCD_RESET_PIN "PA02"
#define KEY_EVENT_DEVICE "/dev/input/event1"
