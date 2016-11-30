/*
 * HwEmulated.hpp
 *
 *  Created on: 22/ago/2016
 *      Author: daniele
 */

#ifndef HWACQUAA5_HPP_
#define HWACQUAA5_HPP_

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

class I2cBusAcquaA5 : public I2cBus {
 private:
  unsigned int active_device;
  map<int, string> filedescriptors;
  // Observers
  using WriteObserver = Subject<void (int filedes, const void *buffer, size_t size)>;

  map<unsigned int, WriteObserver> write_obs_map;  // Write Observer Map by device address

 public:

  I2cBusAcquaA5(const char *bus)
      : I2cBus(bus) {
    active_device = 0;
  }
  ;
  ~I2cBusAcquaA5() {
  }
  ;

  int open(const char *file, int flag);
  int read(int fd, void *buf, size_t nbyte);
  int write(int filedes, const void *buffer, size_t size);
  int ioctl(int fd, unsigned long int __request, ...);
  __off_t lseek(int fd, __off_t   __offset, int __whence);
  int close(int fd);

};

class SysFsAcquaA5 : public SysFs {
 private:

 public:

  SysFsAcquaA5(const char *root)
      : SysFs(root) {
  }
  ;
  ~SysFsAcquaA5() {
  }
  ;

  int open(const char *file, int flag);
  int read(int fd, void *buf, size_t nbyte);
  int write(int filedes, const void *buffer, size_t size);
  int ioctl(int fd, unsigned long int __request, ...);
  __off_t lseek(int fd, __off_t   __offset, int __whence);
  int close(int fd);

};

class GpioPortAcquaA5 : public GpioPort {
 private:

 public:
  GpioPortAcquaA5(const char *name)
      : GpioPort(name) {
  }
  ;
  ~GpioPortAcquaA5() {
  }
  ;

  int open(const char *file, int flag);
  int read(int fd, void *buf, size_t nbyte);
  int write(int filedes, const void *buffer, size_t size);
  int ioctl(int fd, unsigned long int __request, ...);
  __off_t lseek(int fd, __off_t   __offset, int __whence);
  int close(int fd);

};

class BoardAcquaA5 : public Board {
  I2cBusAcquaA5 i2c_0;
  GpioPortAcquaA5 lcd_backlight;
  GpioPortAcquaA5 lcd_reset;

 public:
  BoardAcquaA5()
      : Board(),
        i2c_0(I2C_BUS),
        lcd_backlight(LCD_BACKLIGHT_PIN),
        lcd_reset(LCD_RESET_PIN) {

  }
  ;
  ~BoardAcquaA5() {
  }
  I2cBusAcquaA5& getI2c0() {
    return i2c_0;
  }

  GpioPortAcquaA5& getLcdBacklight() {
    return lcd_backlight;
  }

  GpioPortAcquaA5& getLcdReset() {
    return lcd_reset;
  }

};

}
#endif /* HWACQUAA5_HPP_ */
