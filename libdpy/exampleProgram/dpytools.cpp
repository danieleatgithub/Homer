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
#include <getopt.h>
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
#include <log4cplus/config.hxx>
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
#define no_argument 0
#define required_argument 1
#define optional_argument 2

using namespace log4cplus;
using namespace dpy;

/****************************************************************
 * Main
 ****************************************************************/
int main(int argc, char **argv, char **envp) {
	const char* res;
    int pause = 500000;


	initialize();
	BasicConfigurator config;
	config.configure();
	Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("main"));
	logger.setLogLevel(ERROR_LOG_LEVEL);
	LOG4CPLUS_TRACE(logger, "printMessages()");
	LOG4CPLUS_DEBUG(logger, "This is a DEBUG message");
	LOG4CPLUS_INFO(logger, "This is a INFO message");
	LOG4CPLUS_WARN(logger, "This is a WARN message");
	LOG4CPLUS_ERROR(logger, "This is a ERROR message");
	LOG4CPLUS_FATAL(logger, "This is a FATAL message");

	Dpy display(I2C_BUS, LCD_RESET_PIN, LCD_BACKLIGHT_PIN);

	if (argc != 1) {
		printf("no arguments needed\n");
		exit(-1);
	}

	printf("WinstarDisplay tests .. resetting ...\n");
	display.dpy_open();

	display.set_backlight(false);
	display.set_backlight(true);
	display.reset();
	display.clear();

	display.set_two_lines();
	display.set_one_line();
	display.set_double_height();

	display.set_cursor_on(true);
	display.set_cursor_blink(true);
	display.set_cursor_on(false);
	display.set_cursor_blink(false);

	display.shift_line();
	display.shift_cursor();

	display.home();
	display.clear();
	display.line2_home();

	display.set_contrast(0);
	display.set_contrast(display.get_contrast()+1);
	display.set_contrast(display.get_contrast()-1);
	display.set_contrast(15);

	display.puts("12345678901234567890"); // 20
	display.puts("1234567890"); // 10
	display.puts("Daniele"); // 7
	display.puts("Tiziana");
	display.puts("Claudia");
	display.puts("Luciana");
	display.puts("Agnese"); // 6
	display.puts("Flavia");
	display.puts("Chiara");
	display.puts("Laura"); // 5
	display.puts("Katia");
	display.puts("DO"); // 2
	display.puts("RE"); // 2
	display.puts("MI"); // 2
	display.puts("0"); // 1
	display.puts("1"); // 1
	display.puts("2"); // 1
	display.puts("3"); // 1



	display.dpy_close();

	printf("== END ==");

}
