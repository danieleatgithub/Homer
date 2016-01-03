/*
 * main.cpp
 *
 *  Created on: 19/apr/2015
 *      Author: daniele
 *      Entry start point for homer daemon
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
#include "Lcd.h"
#include "homer.h"

using namespace log4cplus;

/****************************************************************
 * Main
 ****************************************************************/
int main(int argc, char **argv, char **envp)
{


/*
	initialize();
	BasicConfigurator config;
	config.configure();
	Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("main"));
	LOG4CPLUS_WARN(logger, LOG4CPLUS_TEXT("Hello, World!"));
*/
	std::Lcd display(I2C_BUS,LCD_RESET_PIN,LCD_BACKLIGHT_PIN);

	if (argc != 1) {
		printf("no arguments needed\n");
		exit(-1);
	}
	printf("WinstarDisplay tests \n\n");
	printf("Open with Light on ... ");
	if(display.lcd_open() < 0) 				   	printf("KO\n"); else printf("OK\n");

	printf("puts Ciao on Display ... ");
	if(display.lcd_puts((char *)"Ciao") < 0) 	printf("KO\n"); else printf("OK\n");

	printf("Turn off light ... ");
	if(display.setBacklight(STATE_OFF) < 0) 	printf("KO\n"); else printf("OK\n");
	sleep(1);

	printf("Turn on light ... ");
	if(display.setBacklight(STATE_TOGGLE) < 0) 	printf("KO\n"); else printf("OK\n");

	printf("Set Cursor ON and blinking ... ");
	if(display.setCursor(true,true)< 0) 	printf("KO\n"); else printf("OK\n");
	sleep(1);

	printf("Set Cursor ON and not blinking ... ");
	if(display.setCursor(true,false)< 0) 	printf("KO\n"); else printf("OK\n");
	sleep(1);

	printf("Set Cursor off and not blinking ... ");
	if(display.setCursor(true,false)< 0) 	printf("KO\n"); else printf("OK\n");
	sleep(1);

	sleep(10);
	printf("Close .... ");
	if(display.lcd_close() < 0) 				printf("KO\n"); else printf("OK\n");
	printf("== END ==");

}

