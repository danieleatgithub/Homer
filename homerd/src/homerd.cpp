/*
 ============================================================================
 Name        : homerd.cpp
 Author      : Daniele Colombo
 Version     :
 Copyright   : GPL 2
 Description : Hello World in C++,
 ============================================================================
 */


#include <log4cplus/config.hxx>
#include <log4cplus/configurator.h>
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/loglevel.h>
#include <log4cplus/tchar.h>
#include <string>

#include "Display.h"
#include "Fakedisplay.h"
#include "Sysinfo.h"

// FIXME get from properties
#define ONEWIRE_0_PIN   "PA01"
#define I2C_0_DAT_PIN   "PA30"
#define I2C_0_CLK_PIN   "PA31"
#define I2C_BUS                 "/dev/i2c-0"
#define SYSFS_GPIO_DIR  "/sys/class/gpio"
#define LCD_BACKLIGHT_PIN       "PA03"
#define LCD_RESET_PIN           "PA02"
#define BUT_UP_PIN              ""
#define BUT_DOWN_PIN    ""
#define BUT_RIGHT_PIN   ""
#define BUT_LEFT_PIN    ""
#define BUT_ENTER_PIN   ""
#define no_argument 0
#define required_argument 1
#define optional_argument 2

using namespace std;
using namespace log4cplus;
using namespace homerio;

int main(void) {
	Display *display;
	string ip;

	Sysinfo sysinfo = Sysinfo::get_instance();

	initialize();
	BasicConfigurator config;
	config.configure();
	Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("main"));
	logger.setLogLevel(INFO_LOG_LEVEL);
	LOG4CPLUS_INFO(logger, "homerd starting");
	logger.setLogLevel(INFO_LOG_LEVEL);

	display = new FakeDisplay(I2C_BUS, LCD_RESET_PIN, LCD_BACKLIGHT_PIN);
//	display = new Winstar(I2C_BUS, LCD_RESET_PIN, LCD_BACKLIGHT_PIN);

	ip = sysinfo.get_local_ip("eth0");

	display->dpy_open();
	display->set_backlight(false);
	display->dpy_puts("Homer"); // 7
	display->line2_home();
	display->dpy_puts("Homer"); // 7
	display->dpy_puts(ip.c_str());
	display->dpy_close();

	delete(display);
	return 0;
}
