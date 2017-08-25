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

#ifndef HWLAYER_HPP_
#define HWLAYER_HPP_

#include <linux/i2c-dev.h>
#include <stdarg.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <errno.h>
#include <string>
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/loglevel.h>
#include "homerd.h"

using namespace std;
using namespace log4cplus;

namespace homerio {

// https://www.kernel.org/doc/Documentation/i2c/dev-interface
class I2cBus {
 protected:
  const char *bus;

 public:

  I2cBus(const char *bus) {
    this->bus = bus;
  }
  virtual ~I2cBus() {
  }
  ;

  virtual int open(const char *file, int flag) = 0;
  virtual int read(int fd, void *buf, size_t nbyte) = 0;
  virtual int write(int filedes, const void *buffer, size_t size) = 0;
  virtual int close(int fd) = 0;
  virtual int ioctl(int fd, unsigned long int __request, ...) = 0;
  virtual __off_t lseek(int fd, __off_t                                           __offset, int __whence) = 0;

  const char* getBus() const {
    return bus;
  }
};

class SysFs {

 protected:
  const char *root;

 public:

  SysFs(const char *root) {
    this->root = root;
  }
  virtual ~SysFs() {
  }
  ;

  virtual int open(const char *file, int flag) = 0;
  virtual int read(int fd, void *buf, size_t nbyte) = 0;
  virtual int write(int filedes, const void *buffer, size_t size) = 0;
  virtual int close(int fd) = 0;
  virtual int ioctl(int fd, unsigned long int __request, ...) = 0;
  virtual __off_t lseek(int fd, __off_t                                           __offset, int __whence) = 0;
  const char* getRoot() {
    return root;
  }

  int readBuffer(const char *fn, char *buf, int nbytes) {
    int fd;
    int nread, ret;
    string fqfn(string(root) + string(fn));

    Logger logdev = Logger::getInstance(LOGDEVICE);

    fd = this->open(fqfn.c_str(), O_RDONLY);
    if (fd < 0) {
      LOG4CPLUS_ERROR(logdev,
                      "open error " << fqfn << " errno:" << strerror(errno));
      return (fd);
    }
    nread = this->read(fd, buf, nbytes);
    if (nread < 0) {
      LOG4CPLUS_ERROR(logdev,
                      "read error " << fqfn << " errno:" << strerror(errno));
      return (nread);
    }
    buf[nread] = '\0';
    ret = this->close(fd);
    if (ret < 0) {
      LOG4CPLUS_ERROR(logdev,
                      "close error " << fqfn << " errno:" << strerror(errno));
      return (ret);
    } else {
      return (nread);
    }

  }
};

class GpioPort {
 protected:
  const char *name;
 public:
  GpioPort(const char *name) {
    this->name = name;
  }
  ;
  virtual ~GpioPort() {
  }
  ;

  virtual int open(const char *file, int flag) = 0;
  virtual int read(int fd, void *buf, size_t nbyte) = 0;
  virtual int write(int filedes, const void *buffer, size_t size) = 0;
  virtual int close(int fd) = 0;
  virtual int ioctl(int fd, unsigned long int __request, ...) = 0;
  virtual __off_t lseek(int fd, __off_t                                           __offset, int __whence) = 0;
  const char* getName() const {
    return name;
  }
};

class Board {
 public:
  Board() {
  }
  ;
  virtual ~Board() {
  }
  ;
  virtual I2cBus& getI2c0() = 0;
  virtual I2cBus& getI2c3() = 0;
  virtual GpioPort& getLcdBacklight() = 0;
  virtual GpioPort& getLcdReset() = 0;
  virtual SysFs& getSysFs() = 0;

};

}
#endif /* HWLAYER_HPP_ */
