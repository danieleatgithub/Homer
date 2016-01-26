#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <string>
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/configurator.h>
#include <iomanip>
#include "fakedisplay.h"


using namespace std;

namespace homerio {

FakeDisplay::FakeDisplay(const char *bus) : Winstar(bus) {

}
FakeDisplay::FakeDisplay(const char *bus, const char *rst, const char *backlight) : Winstar(bus) {

}

FakeDisplay::~FakeDisplay() {
}

int FakeDisplay::dpy_open() {

	return(device_init());
}

int FakeDisplay::dpy_close() {

	return (0);
}


int FakeDisplay::dpy_write(int type, uint8_t data) {
	if(type == WSTAR_CMD) {
		printf("DEBUG i2cset -y 0x%x 0x%x 0x%x\n", address, type, data);
	} else {
		cout << data;
	}
	return (0);
}



}

