/*******************************************************************************
 * Copyright (C) 2016  Daniele Colombo
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *******************************************************************************/

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

/**
 * I2cBus emulation
 * Used for:
 *  Display emulator
 */
class I2cBusEmulated : public I2cBus {
 private:
  unsigned int active_device;  // The last device selected with ioctl I2C_SLAVE
  map<int, string> filedescriptors;

  using WriteObserver = Subject<void (int filedes, const void *buffer, size_t size)>;
  map<unsigned int, WriteObserver> write_obs_map;  // Write Observer Map by device address

 public:

  I2cBusEmulated(const char* const bus)
      : I2cBus(bus) {
    active_device = 0;
  }
  ;
  ~I2cBusEmulated() {
  }
  ;

  // Overloaded i2c function
  int open(const char *file, int flag);
  int read(int fd, void *buf, size_t nbyte);
  int write(int filedes, const void *buffer, size_t size);
  int ioctl(int fd, unsigned long int __request, ...);
  __off_t lseek(int fd, __off_t                __offset, int __whence);
  int close(int fd);

  /**
   * Register a write observer callback on a specific address
   * @param reg Reference to registration
   * @param address  Address example: WINSTAR_I2C_ADD
   * @param f callback void(int filedes, const void *buffer, size_t size)
   */
  void reg_write(
      Registration& reg, unsigned int address,
      std::function<void(int filedes, const void *buffer, size_t size)> f) {
    reg = write_obs_map[address].registerObserver(f);
  }
  // TODO: reg_read
  // TODO: copy from the sysemulaton the ret value
};

/**
 * sys filesystem emulation
 * Used for:
 *    bmp85 sensor
 */
class SysFsEmulated : public SysFs {
 private:
  unsigned int active_device;
  map<int, string> filedescriptors;

  // Observers
  using ReadObserver = Subject<void (int filedes, void *buffer, size_t size, const char *fname, int *ret)>;
  using WriteObserver = Subject<void (int filedes, const void *buffer, size_t size, const char *fname, int *ret)>;

  map<string, WriteObserver> write_obs_map;  // Write Observer Map by fd
  map<string, ReadObserver> read_obs_map;  // Read Observer Map by fd

 public:

  SysFsEmulated(const char * const root)
      : SysFs(root) {
    active_device = 0;
  }
  ;
  ~SysFsEmulated() {
  }
  ;

  int open(const char *file, int flag);
  int read(int fd, void *buf, size_t nbyte);
  int write(int filedes, const void *buffer, size_t size);
  int ioctl(int fd, unsigned long int __request, ...);
  __off_t lseek(int fd, __off_t                __offset, int __whence);
  int close(int fd);

  /**
   * Register a write observer callback on a specific file pattern
   * @param reg Reference to registration
   * @param filePattern regex pattern to trigger the observer
   * @param f callback (int filedes, const void *buffer, size_t size, const char *fname,
   int *ret) in ret the emulated return value
   */
  void reg_write(
      Registration& reg,
      string filePattern,
      std::function<
          void(int filedes, const void *buffer, size_t size, const char *fname,
               int *ret)> f) {
    reg = write_obs_map[filePattern].registerObserver(f);
  }

  void reg_read(
      Registration& reg,
      string filePattern,
      std::function<
          void(int filedes, void *buffer, size_t size, const char *fname,
               int *ret)> f) {
    reg = read_obs_map[filePattern].registerObserver(f);
  }
};

class GpioPortEmulated : public GpioPort {
 private:
  // Observers
  Subject<void(int filedes, const void *buffer, size_t size)> write_obs;  // Triggered on key released

 public:
  GpioPortEmulated(const char *name)
      : GpioPort(name) {
  }
  ;
  ~GpioPortEmulated() {
  }
  ;

  int open(const char *file, int flag);
  int read(int fd, void *buf, size_t nbyte);
  int write(int filedes, const void *buffer, size_t size);
  int ioctl(int fd, unsigned long int __request, ...);
  __off_t lseek(int fd, __off_t                                                        __offset, int __whence);
  int close(int fd);

  void reg_write(
      Registration& reg,
      std::function<void(int filedes, const void *buffer, size_t size)> f) {
    reg = write_obs.registerObserver(f);
  }

};

class BoardEmulated : public Board {
  I2cBusEmulated i2c_0;
  GpioPortEmulated lcd_backlight;
  GpioPortEmulated lcd_reset;
  SysFsEmulated sysFs;

 public:
  BoardEmulated()
      : Board(),
        i2c_0(I2C_BUS_0),
        lcd_backlight(LCD_BACKLIGHT_PIN),
        lcd_reset(LCD_RESET_PIN),
        sysFs(SYSFS_ROOT) {

  }
  ;
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

  SysFsEmulated& getEmulatedSysFs() {
    return sysFs;
  }
  SysFs& getSysFs() {
    return sysFs;
  }

};

}
#endif /* HWEMULATED_HPP_ */
