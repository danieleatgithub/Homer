/*
 ============================================================================
 Name        : exampleProgram.c
 Author      : Daniele Colombo
 Version     :
 Copyright   : GPL 2
 Description : Uses shared library to print greeting
 To run the resulting executable the LD_LIBRARY_PATH must be
 set to ${project_loc}/libdpy/.libs
 Alternatively, libtool creates a wrapper shell script in the
 build directory of this program which can be used to run it.
 Here the script will be called exampleProgram.
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <iostream>
#include <string>
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/configurator.h>
#include <iomanip>
#include "libdpy.h"

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

using namespace log4cplus;
using namespace dpy;

/****************************************************************
 * Main
 ****************************************************************/
int main(int argc, char **argv, char **envp) {
	printf("ciao");

	initialize();
	BasicConfigurator config;
	config.configure();
	Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("main"));
	logger.setLogLevel(DEBUG_LOG_LEVEL);
	LOG4CPLUS_TRACE(logger, "printMessages()");
	LOG4CPLUS_DEBUG(logger, "This is a DEBUG message");
	LOG4CPLUS_INFO(logger, "This is a INFO message");
	LOG4CPLUS_WARN(logger, "This is a WARN message");
	LOG4CPLUS_ERROR(logger, "This is a ERROR message");
	LOG4CPLUS_FATAL(logger, "This is a FATAL message");

	Lcd display(I2C_BUS, LCD_RESET_PIN, LCD_BACKLIGHT_PIN);

	if (argc != 1) {
		printf("no arguments needed\n");
		exit(-1);
	}
	printf("WinstarDisplay tests \n\n");
	printf("Open with Light on ... ");
	if (display.lcd_open() < 0)
		printf("KO\n");
	else
		printf("OK\n");

	printf("puts Ciao on Display ... ");
	if (display.lcd_puts((char *) "Ciao") < 0)
		printf("KO\n");
	else
		printf("OK\n");

	printf("Turn off light ... ");
	if (display.setBacklight(STATE_OFF) < 0)
		printf("KO\n");
	else
		printf("OK\n");
	sleep(1);

	printf("Turn on light ... ");
	if (display.setBacklight(STATE_TOGGLE) < 0)
		printf("KO\n");
	else
		printf("OK\n");

	printf("Set Cursor ON and blinking ... ");
	if (display.setCursor(true, true) < 0)
		printf("KO\n");
	else
		printf("OK\n");
	sleep(1);

	printf("Set Cursor ON and not blinking ... ");
	if (display.setCursor(true, false) < 0)
		printf("KO\n");
	else
		printf("OK\n");
	sleep(1);

	printf("Set Cursor off and not blinking ... ");
	if (display.setCursor(true, false) < 0)
		printf("KO\n");
	else
		printf("OK\n");
	sleep(1);

	sleep(1);
	printf("Close .... ");
	if (display.lcd_close() < 0)
		printf("KO\n");
	else
		printf("OK\n");
	printf("== END ==");

}
