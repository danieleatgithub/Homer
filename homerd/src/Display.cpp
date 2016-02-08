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

using namespace std;
using namespace log4cplus;

namespace homerio {
struct pinmap_s;

Display::Display(const char *bus) {
    init();
    this->rst = string("");
    this->bus = string(bus);
    this->backlight = string("");
    this->backlight_state = false;
    this->reset_pin = new GpioPin();
    this->backlight_pin = new GpioPin();
}

Display::Display(const char *bus, const char *rst, const char *backlight) {
    struct pinmap_s *pin;
    init();
    this->bus = string(bus);
    this->rst = string(rst);
    this->backlight = string(backlight);
    this->backlight_state = true;

    pin = GpioPin::getPinDescriptor(this->rst.c_str());
    reset_pin = new GpioPin(pin);
    reset_pin->pin_export();
    reset_pin->set_direction(homerio::OUT);
    reset_pin->pin_open();
    reset_pin->setState(homerio::STATE_ON);

    pin = GpioPin::getPinDescriptor(this->backlight.c_str());
    backlight_pin = new GpioPin(pin);
    backlight_pin->pin_export();
    backlight_pin->set_direction(homerio::OUT);
    backlight_pin->pin_open();
}

Display::~Display() {
    dpy_close();
    reset_pin->pin_close();
    backlight_pin->pin_close();
    delete (reset_pin);
    delete (backlight_pin);
}

void Display::init() {
    this->write_usleep = 100;
    this->fd = -1;
    this->address = 0xff;
}

int Display::dpy_open() {

    // Open i2c
	Logger logdev = Logger::getInstance(LOGDEVICE);
    fd = open(bus.c_str(), O_RDWR);
    if (fd < 0) return (fd);
    if (ioctl(fd, I2C_SLAVE, this->address) < 0) {
    	LOG4CPLUS_ERROR(logdev, "ioctl error: " << strerror(errno) << "\n");
        close(fd);
        return -1;
    }
    return (device_init());
}

int Display::dpy_close() {
    int ret;
    ret = close(this->fd);
    return (ret);
}

int Display::reset() {
    reset_pin->flip(1000);  // 1 milli
    usleep(100000);         // 100 milli
    return (device_init());
}
int Display::set_backlight(bool state) {
    return (backlight_pin->setState((state ? STATE_ON : STATE_OFF)));
}

int Display::set_backlight(bool state, unsigned int delay_ms) {
    std::thread([this, state, delay_ms] {
        std::this_thread::sleep_for(
        std::chrono::milliseconds{delay_ms});
        set_backlight(state);
    }).detach();
    return (0);
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
