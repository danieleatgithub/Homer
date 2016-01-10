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

	if(display.dpy_reset() < 0)
		printf("KO\n");
	else
		printf("OK\n");

	printf("Open with Light on ... ");
	if (display.dpy_open() < 0)
		printf("KO\n");
	else
		printf("OK\n");

	printf("puts Ciao on Display ... ");
	if (display.dpy_puts((char *) "Ciao") < 0)
		printf("KO\n");
	else
		printf("OK\n");

	printf("Turn off light ... ");
	if (display.set_backlight(STATE_OFF) < 0)
		printf("KO\n");
	else
		printf("OK\n");
	usleep(pause);

	printf("Turn on light ... ");
	if (display.set_backlight(STATE_TOGGLE) < 0)
		printf("KO\n");
	else
		printf("OK\n");

	printf("Set Cursor ON and blinking ... ");
	res = "KO";
	if (display.set_cursor(true, true) == 0)
		if(display.is_cursor_on()) res = "OK";
	printf("%s\n",res);
	usleep(pause);

	printf("Set Cursor ON and not blinking ... ");
	res = "KO";
	if (display.set_cursor(true, false) == 0)
		if(display.is_cursor_on()) res = "OK";
	printf("%s\n",res);
	usleep(pause);

	printf("Set Cursor off and not blinking ... ");
	res = "KO";
	if (display.set_cursor(false, false) == 0)
		if(!display.is_cursor_on()) res = "OK";
	printf("%s\n",res);
	usleep(pause);

	printf("Set Display off  ... ");
	res = "KO";
	if (display.set_state(STATE_OFF) == 0)
		if(!display.is_display_on()) res = "OK";
	printf("%s\n",res);
	usleep(pause);

	printf("Set Display on  ... ");
	res = "KO";
	if (display.set_state(STATE_ON) == 0)
		if(display.is_display_on()) res = "OK";
	printf("%s\n",res);
	usleep(pause);

	printf("Set Display toggle  ... ");
	res = "KO";
	if (display.set_state(STATE_TOGGLE) == 0)
		if(!display.is_display_on())  {
			usleep(pause);
			if (display.set_state(STATE_TOGGLE) == 0)
				if(display.is_display_on()) res = "OK";
		}
	printf("%s\n",res);
	usleep(pause);


	printf("Set Contrast on  ... ");
	res = "KO";
	if (display.set_contrast(0) == 0)
		if(display.get_contrast() == 0) res = "OK";
	printf("%s\n",res);
	usleep(pause);

	printf("Home ... ");
	res = "KO";
	if (display.set_cursor(true, true) == 0)
		if(display.home()) res = "OK";
	printf("%s\n",res);
	usleep(pause);

	printf("Clear ... ");
	res = "KO";
	if(display.clear()) res = "OK";
	printf("%s\n",res);
	usleep(pause);

	printf("Write 0123456789ABCDEFGHILMNOPQ ... ");
	res = "KO";
	if(display.dpy_puts("0123456789ABCDEFGHILMNOPQ")) res = "OK";
	printf("%s\n",res);
	usleep(pause);

	printf("Enable shift 0123456789 .....");
	res = "KO";
	display.clear(true);
	if(display.set_shift(STATE_ON )) res = "OK";
	for (int i=(int)'A'; i<(int)'Q';i++) {
		display.dpy_putchar((unsigned char)i);
		usleep(600000);
	}
	printf("%s\n",res);


	usleep(pause);

	sleep(1);
	printf("Close .... ");
	if (display.dpy_close() < 0)
		printf("KO\n");
	else
		printf("OK\n");
	printf("== END ==");

}
