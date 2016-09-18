/*
 * HwEmulated.hpp
 *
 *  Created on: 22/ago/2016
 *      Author: daniele
 */

#ifndef HWEMULATED_HPP_
#define HWEMULATED_HPP_

#include <HwLayer.hpp>
#include <linux/i2c-dev.h>
#include <stdarg.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <errno.h>
#include "Observer.h"
#include <homerd.h>
#include <map>

using namespace obs;

namespace homerio {

class I2cBusEmulated : public I2cBus {
private:
	unsigned int active_device;
	map<int,string> filedescriptors;
    // Observers
	using WriteObserver = Subject<void (int filedes, const void *buffer, size_t size)>;

	map<unsigned int,WriteObserver> write_obs_map; // Write Observer Map by device address

public:

	I2cBusEmulated(const char *bus) : I2cBus(bus) {
		active_device = 0;
	};
	 ~I2cBusEmulated() {};

	int open(const char *file, int flag);
	int read(int fd, void *buf, size_t nbyte);
	int write(int filedes, const void *buffer, size_t size);
	int ioctl(int fd, unsigned long int __request, ...);
	__off_t lseek (int fd, __off_t __offset, int __whence);
	int close(int fd);

	void reg_write(Registration& reg, unsigned int address, std::function<void (int filedes, const void *buffer, size_t size)> f) {
		reg = write_obs_map[address].registerObserver(f);
	}

};

class GpioPortEmulated : public GpioPort  {
private:
    // Observers
    Subject<void (int filedes, const void *buffer, size_t size)> write_obs; // Triggered on key released


public:
	 GpioPortEmulated(const char *name) : GpioPort(name) {};
	 ~GpioPortEmulated() {};

		int open(const char *file, int flag);
		int read(int fd, void *buf, size_t nbyte);
		int write(int filedes, const void *buffer, size_t size);
		int ioctl(int fd, unsigned long int __request, ...);
		__off_t lseek (int fd, __off_t __offset, int __whence);
		int close(int fd);

		void reg_write(Registration& reg, std::function<void (int filedes, const void *buffer, size_t size)> f) {
	    	reg = write_obs.registerObserver(f);
		}

};



class BoardEmulated : public Board {
	I2cBusEmulated    i2c_0;
	GpioPortEmulated  lcd_backlight;
	GpioPortEmulated  lcd_reset;

public:
	BoardEmulated() : Board(),
		i2c_0(I2C_BUS), lcd_backlight(LCD_BACKLIGHT_PIN), lcd_reset(LCD_RESET_PIN)  {

	};
	~BoardEmulated() {
	}
	I2cBus& getI2c0() {
		return i2c_0;
	}

	GpioPort& getLcdBacklight() {
		return lcd_backlight;
	}

	GpioPort& getLcdReset() {
		return lcd_reset;
	}


	I2cBusEmulated& getEmulatedI2c0() {
		return i2c_0;
	}
	GpioPortEmulated& getEmulatedLcdBacklight() {
		return lcd_backlight;
	}

	GpioPortEmulated& getEmulatedLcdReset() {
		return lcd_reset;
	}

};

}
#endif /* HWEMULATED_HPP_ */
