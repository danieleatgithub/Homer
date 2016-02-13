/*
 * pin.cpp
 *
 *  Created on: 06/gen/2016
 *      Author: daniele
 */

#include <fcntl.h>
#include <GpioPin.h>
#include <string.h>
#include <unistd.h>
#include <cstdio>
#include <string>
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/loglevel.h>
#include "homerd.h"

const char *Direction_s[2] = { "out", "in" };
const char *Edge_s[4] = { "rising", "falling", "none", "both" };

using namespace std;
using namespace log4cplus;

namespace homerio {

static struct pinmap_s {
    unsigned int kernel_id;
    const char *kernel_string;
    const char *name;
} pinmap[] = { 0, "pioA0", "PA00",
				1, "pioA1", "PA01",
				2, "pioA2", "PA02",
				3, "pioA3", "PA03",
				6, "pioA6", "PA06",
				77, "pioC13", "PC13"
             };

GpioPin::~GpioPin() {
    // TODO Auto-generated destructor stub
}
void GpioPin::init() {
    this->kstr = "";
    this->kid = -1;
    this->name = "";
    this->fd = -1;
    this->direction = INP;
    this->edge = RISING;
    this->in_use = false;
    this->last_value = false;
}

GpioPin::GpioPin() {
    init();
}
GpioPin::GpioPin(struct pinmap_s *pin_desc) {
    init();
    if (pin_desc) {
        this->kstr = pin_desc->kernel_string;
        this->kid = pin_desc->kernel_id;
        this->name = pin_desc->name;
    }
}

GpioPin::GpioPin(int kid, string kstr, string name) {
    init();
    this->kstr = kstr;
    this->kid = kid;
    this->name = name;
}

struct pinmap_s *GpioPin::getPinDescriptor(const char *name) {
    unsigned int i = 0;
    for (i = 0; i < (sizeof(pinmap) / sizeof(struct pinmap_s)); i++) {
        if (strcmp(pinmap[i].name, name) == 0) {
            return (&pinmap[i]);
        }
    }
    return (NULL);
}

int GpioPin::get_pin() {
    return (this->kid);
}

int GpioPin::pin_export() {
    int fd, len;
    char buf[MAX_BUF];
    Logger logdev = Logger::getInstance(LOGDEVICE);
    if (kid < 0)
        return (-1);
    if (this->in_use) {
        LOG4CPLUS_ERROR(logdev,"Pin " << name << " already in use");
        return -2;
    }
    fd = open(SYSFS_GPIO_DIR "/export", O_WRONLY);
    if (fd < 0) {
        LOG4CPLUS_ERROR(logdev,"Pin " << name << " fail open for export");
        return fd;
    }
    len = snprintf(buf, sizeof(buf), "%d", this->kid);
    LOG4CPLUS_DEBUG(logdev,"echo " << SYSFS_GPIO_DIR << "/export " << buf);
    write(fd, buf, len);
    close(fd);
    this->in_use = true;
    return 0;

}
int GpioPin::pin_unexport() {
    int fd, len;
    char buf[MAX_BUF];
    Logger logdev = Logger::getInstance(LOGDEVICE);
    if (kid < 0)
        return (-1);
    if (!this->in_use) {
        LOG4CPLUS_ERROR(logdev,"Pin " << name << " not in use");
        return -100;
    }
    if (this->fd > 0) {
        close(this->fd);
    }
    fd = open(SYSFS_GPIO_DIR "/unexport", O_WRONLY);
    if (fd < 0) {
        LOG4CPLUS_ERROR(logdev,"Pin " << name << " fail open for unexport");
        return fd;
    }

    len = snprintf(buf, sizeof(buf), "%d", this->kid);
    LOG4CPLUS_DEBUG(logdev,"echo " << SYSFS_GPIO_DIR << "/unexport " << buf);
    write(fd, buf, len);
    close(fd);
    this->in_use = false;
    return 0;

}
int GpioPin::set_direction(Direction_e dir) {
    int fd, ret;
    char buf[MAX_BUF];
    Logger logdev = Logger::getInstance(LOGDEVICE);
    if (kid < 0)
        return (-1);
    if (!this->in_use) {
        LOG4CPLUS_ERROR(logdev,"Pin " << name << " not in use");
        return -100;
    }

    snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/%s/direction",
             this->kstr.c_str());

    fd = open(buf, O_WRONLY);
    if (fd < 0) {
        LOG4CPLUS_ERROR(logdev,"Pin " << name << " fail open for direction");
        return fd;
    }

    LOG4CPLUS_DEBUG(logdev,"echo " << buf << " " << Direction_s[dir]);
    ret = write(fd, Direction_s[dir], (strlen(Direction_s[dir]) + 1));

    close(fd);
    if (ret < 0) {
        return (ret);
    }
    this->direction = dir;
    // Default out value is low
    if (this->direction == OUT)
        this->last_value = false;
    return 0;
}
int GpioPin::set_edge(Edge_e edge) {
    int fd, ret;
    char buf[MAX_BUF];
    Logger logdev = Logger::getInstance(LOGDEVICE);
    if (kid < 0)
        return (-1);
    if (!this->in_use) {
        LOG4CPLUS_ERROR(logdev,"Pin " << name << " not in use");
        return -100;
    }

    snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/%s/edge", this->kstr.c_str());

    fd = open(buf, O_WRONLY);
    if (fd < 0) {
        LOG4CPLUS_ERROR(logdev,"Pin " << name << " fail open for set-edge");
        return fd;
    }
    LOG4CPLUS_DEBUG(logdev,"echo " << SYSFS_GPIO_DIR << "/"
    		<< this->kstr.c_str() << "/edge " << Edge_s[edge]);
    ret = write(fd, Edge_s[edge], strlen(Edge_s[edge]) + 1);
    close(fd);
    if (ret < 0) {
        return (ret);
    }
    return 0;
}

int GpioPin::setState(State_e state) {
    bool newval;
    if (kid < 0)
        return (-1);
    if (state == STATE_OFF || (last_value && state == STATE_TOGGLE))
        newval = false;
    if (state == STATE_ON || (!last_value && state == STATE_TOGGLE))
        newval = true;
    if (write_value(newval) < 0)
        return (-1);
    last_value = newval;
    return (0);
}
int GpioPin::getfd() {
	return(this->fd);
}
int GpioPin::flip(unsigned int us) {
    if (kid < 0)
        return (-1);
    setState(STATE_TOGGLE);
    usleep(us);
    return (setState(STATE_TOGGLE));
}
int GpioPin::write_value(bool value) {
    int fd;
    char buf[MAX_BUF];
    const char *val;
    Logger logdev = Logger::getInstance(LOGDEVICE);

    if (value) val = "1";
    else       val = "0";

    LOG4CPLUS_DEBUG(logdev,"echo " << SYSFS_GPIO_DIR << "/"
        		<< this->kstr.c_str() << "/value " << val);

    if (kid < 0)
        return (-1);
    if (!this->in_use || this->direction != OUT) {
    	LOG4CPLUS_ERROR(logdev,"Pin " << name << " not in use");
        return -100;
    }
    if (this->fd <= 0) {
        snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/%s/value",
                 this->kstr.c_str());

        fd = open(buf, O_WRONLY);
        if (fd < 0) {
        	LOG4CPLUS_ERROR(logdev,"Pin " << name << " fail open for set value");
            return fd;
        }
        write(fd, val, 2);
        close(fd);
    } else {
        write(this->fd, val, 2);
        lseek(this->fd, 0, SEEK_SET);
    }
    this->last_value = value;
    return 0;
}

/****************************************************************
 * gpio_get_value
 ****************************************************************/
int GpioPin::get(bool *value) {
    int fd;
    char buf[MAX_BUF];
    char ch;
    Logger logdev = Logger::getInstance(LOGDEVICE);

    if (kid < 0)
        return (-1);
    //len = snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/%s/value", this->kstr.c_str());

    fd = open(buf, O_RDONLY);
    if (fd < 0) {
    	LOG4CPLUS_ERROR(logdev,"Pin " << name << " fail open for get value");
        return fd;
    }

    read(fd, &ch, 1);

    if (ch != '0') {
        *value = true;
    } else {
        *value = false;
    }

    close(fd);
    return 0;
}
int GpioPin::pin_open() {
    char buf[MAX_BUF];
    Logger logdev = Logger::getInstance(LOGDEVICE);

    if (kid < 0)
        return (-1);
    snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/%s/value", this->kstr.c_str());

    if (this->direction == OUT)
        this->fd = open(buf, O_WRONLY | O_NONBLOCK);
    else
        this->fd = open(buf, O_RDONLY | O_NONBLOCK);
    if (fd < 0) {
    	LOG4CPLUS_ERROR(logdev,"Pin " << name << " fail pin open ");
    }
    return this->fd;
}

int GpioPin::pin_close() {
    if (kid < 0)
        return (-1);
    return close(this->fd);
}

}
