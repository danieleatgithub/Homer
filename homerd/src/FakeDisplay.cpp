#include <Fakedisplay.h>
#include <stdio.h>
#include <cstdint>
#include <iostream>
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/loglevel.h>
#include "homerd.h"

using namespace std;
using namespace log4cplus;

namespace homerio {

FakeDisplay::FakeDisplay(const char *bus) :
    Winstar(bus) {

}
FakeDisplay::FakeDisplay(const char *bus, const char *rst,
                         const char *backlight) :
    Winstar(bus) {

}

FakeDisplay::~FakeDisplay() {
}

int FakeDisplay::dpy_open() {
    return (device_init());
}

int FakeDisplay::dpy_close() {
    return (0);
}

int FakeDisplay::dpy_write(int type, uint8_t data) {
    Logger logdev = Logger::getInstance(LOGDEVICE);
    if (type == WSTAR_CMD) {
        LOG4CPLUS_DEBUG(logdev,"FakeDisplay i2cset -y 0 0x " << std::hex << address << " 0x" <<  std::hex << type << " 0x" << std::hex << data);
     } else {
    	 LOG4CPLUS_TRACE(logdev,data);
    }
    return (0);
}

}

