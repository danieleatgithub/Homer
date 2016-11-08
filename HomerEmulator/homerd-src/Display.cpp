#include <Display.h>
#include <errno.h>
#include <fcntl.h>
#include <GpioPin.h>
#include <linux/i2c-dev.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <cstdio>
#include <iostream>
#include <chrono>
#include <thread>
#include <future>
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/loglevel.h>
#include "homerd.h"
#include <errno.h>
#include "Observer.h"
#include "KeyPanel.hpp"
#include "Scheduler.hpp"
#include <HwLayer.hpp>

using namespace std;
using namespace log4cplus;
using namespace shd;

namespace homerio {
struct pinmap_s;


Display::Display(KeyPanel &kpnl, Scheduler &shd,I2cBus& bus, GpioPort& rst, GpioPort& backlight) : keyPanel(kpnl), scheduler(shd), i2cBus(bus) {
    this->write_usleep = 100;
    this->address = 0xff;
    this->bus = string(i2cBus.getBus());
    this->rst = string(rst.getName());
    this->backlight = string(backlight.getName());
    this->backlight_state = true;
    this->key_light_delay = 1000;

    timedLightOff.setCallback([&] () { this->set_backlight(false);});
    timedLightOff.setInterval(10);

    reset_pin = new GpioPin(rst);
    reset_pin->pin_export();
    reset_pin->set_direction(homerio::OUT);
    reset_pin->pin_open();
    reset_pin->setState(homerio::STATE_ON);

    backlight_pin = new GpioPin(backlight);
    backlight_pin->pin_export();
    backlight_pin->set_direction(homerio::OUT);
    backlight_pin->pin_open();
	scheduler.ScheduleAfter(timedLightOff);
	keyPanel.key_attach(keyPanel_reg, [&] ( KeyButton& k ) {
		if(!is_backlight_on()) {
			 set_backlight(true);
		}
		scheduler.ScheduleRestart(timedLightOff);
	});
}


Display::~Display() {
    reset_pin->pin_close();
    backlight_pin->pin_close();
    delete (reset_pin);
    delete (backlight_pin);
}



//int Display::dpy_init() {
//
//    // Open i2c
//	Logger logdev = Logger::getInstance(LOGDEVICE);
//    fd = i2cBus.open(bus.c_str(), O_RDWR);
//    if (fd < 0) return (fd);
//    if (i2cBus.ioctl(fd, I2C_SLAVE, this->address) < 0) {
//    	LOG4CPLUS_ERROR(logdev, __PRETTY_FUNCTION__ << "ioctl error: " << strerror(errno) << "\n");
//    	i2cBus.close(fd);
//        return -1;
//    }
//    return (device_init());
//}




int Display::set_backlight(bool state) {
	int ret;
	ret = backlight_pin->setState(state ? STATE_ON : STATE_OFF);
	if(ret >= 0) {
		backlight_state = state;
	}
    return (ret);
}

int Display::dpy_putchar(unsigned char ch) {
    return (this->write_data(ch));
}
int Display::dpy_puts(const char *str) {
    const char *p = str;
    unsigned int ret = 0;
    while (ret == 0 && p && *p) ret = write_data(*p++);
    return ((p ? (p - str) : -1));
}
bool Display::is_backlight_on() {
    return (backlight_state);
}
}
