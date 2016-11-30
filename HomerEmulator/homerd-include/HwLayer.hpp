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
  virtual ~I2cBus() {
  }
  ;

  virtual int open(const char *file, int flag) = 0;
  virtual int read(int fd, void *buf, size_t nbyte) = 0;
  virtual int write(int filedes, const void *buffer, size_t size) = 0;
  virtual int close(int fd) = 0;
  virtual int ioctl(int fd, unsigned long int __request, ...) = 0;
  virtual __off_t lseek(int fd, __off_t                    __offset, int __whence) = 0;

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
  virtual __off_t lseek(int fd, __off_t                    __offset, int __whence) = 0;
  const char* getRoot() {
    return root;
  }

  int readBuffer(const char *fn, char *buf, int nbytes) {
    int fd;
    int nread, ret;
    fd = this->open(fn, O_RDONLY);
    if (fd < 0) {
      return (fd);
    }
    nread = this->read(fd, buf, nbytes);
    if (nread < 0) {
      return (nread);
    }
    buf[nread] = '\0';
    ret = this->close(fd);
    if (ret < 0) {
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
  virtual __off_t lseek(int fd, __off_t                    __offset, int __whence) = 0;
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
  virtual GpioPort& getLcdBacklight() = 0;
  virtual GpioPort& getLcdReset() = 0;
  virtual SysFs& getSysFs() = 0;

};

}
#endif /* HWLAYER_HPP_ */
