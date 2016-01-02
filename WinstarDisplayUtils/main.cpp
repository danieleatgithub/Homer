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
#include "Lcd.h"
#include "homer.h"


/****************************************************************
 * Main
 ****************************************************************/
int main(int argc, char **argv, char **envp)
{

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
	sleep(10);
	printf("Close .... ");
	if(display.lcd_close() < 0) 				printf("KO\n"); else printf("OK\n");
	printf("== END ==");

}

