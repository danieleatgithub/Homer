/*
 * HwLayer.hpp
 *
 *  Created on: 22/ago/2016
 *      Author: daniele
 */

#ifndef HWLAYER_HPP_
#define HWLAYER_HPP_

#include <linux/i2c-dev.h>
#include <stdarg.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <errno.h>

namespace homerio {

// https://www.kernel.org/doc/Documentation/i2c/dev-interface
class I2cBus {
protected:
	const char *bus;

public:

	 I2cBus(const char *bus) {
		 this->bus = bus;
	 }
	virtual ~I2cBus() {};

	virtual int open(const char *file, int flag) = 0;
	virtual int read(int fd, void *buf, size_t nbyte) = 0;
	virtual int write(int filedes, const void *buffer, size_t size) = 0;
	virtual int close(int fd) = 0;
	virtual int ioctl(int fd, unsigned long int __request, ...) = 0;
	virtual __off_t lseek (int fd, __off_t __offset, int __whence) = 0;

	const char* getBus() const {
		return bus;
	}
};

class GpioPort {
protected:
	const char *name;
public:
	GpioPort(const char *name){
		 this->name = name;
	 };
	virtual ~GpioPort() {};

	virtual int open(const char *file, int flag) = 0;
	virtual int read(int fd, void *buf, size_t nbyte) = 0;
	virtual int write(int filedes, const void *buffer, size_t size) = 0;
	virtual int close(int fd) = 0;
	virtual int ioctl(int fd, unsigned long int __request, ...) = 0;
	virtual __off_t lseek (int fd, __off_t __offset, int __whence) = 0;
	const char* getName() const {
		return name;
	}
};


class Board {
public:
	Board() {};
	virtual ~Board() {};
	virtual I2cBus& getI2c0() = 0;
	virtual GpioPort& getLcdBacklight() = 0;
	virtual GpioPort& getLcdReset() = 0;

};


}
#endif /* HWLAYER_HPP_ */
