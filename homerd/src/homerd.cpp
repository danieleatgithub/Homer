/*
 ============================================================================
 Name        : homerd.cpp
 Author      : Daniele Colombo
 Version     :
 Copyright   : GPL 2
 Description : Hello World in C++,
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
#include <sys/types.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <iostream>
#include <string>
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/configurator.h>
#include <iomanip>
#include <winstar.h>
#include <fakedisplay.h>

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
	cout << "homerd starting" << endl; /* prints Hello World */
	Display *display;
	string ip;

	display = new FakeDisplay(I2C_BUS, LCD_RESET_PIN, LCD_BACKLIGHT_PIN);
//	display = new Winstar(I2C_BUS, LCD_RESET_PIN, LCD_BACKLIGHT_PIN);

	display->dpy_open();
	display->set_backlight(false);
	display->dpy_puts("Homer"); // 7

    struct ifaddrs * ifAddrStruct=NULL;
    struct ifaddrs * ifa=NULL;
    void * tmpAddrPtr=NULL;

    getifaddrs(&ifAddrStruct);

    for (ifa = ifAddrStruct; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa ->ifa_addr->sa_family==AF_INET) { // check it is IP4
            // is a valid IP4 Address
            tmpAddrPtr=&((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;
            char addressBuffer[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
            if(strcmp(ifa->ifa_name,"eth0") == 0) ip = string(addressBuffer);
            printf("'%s': %s\n", ifa->ifa_name, addressBuffer);
         } else if (ifa->ifa_addr->sa_family==AF_INET6) { // check it is IP6
            // is a valid IP6 Address
            tmpAddrPtr=&((struct sockaddr_in6 *)ifa->ifa_addr)->sin6_addr;
            char addressBuffer[INET6_ADDRSTRLEN];
            inet_ntop(AF_INET6, tmpAddrPtr, addressBuffer, INET6_ADDRSTRLEN);
            printf("'%s': %s\n", ifa->ifa_name, addressBuffer);
        }
    }
    if (ifAddrStruct!=NULL)
        freeifaddrs(ifAddrStruct);//remember to free ifAddrStruct

	display->line2_home();
	display->dpy_puts(ip.c_str());
	display->dpy_close();

	delete(display);
	return 0;
}
