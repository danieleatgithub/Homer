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
#include <Winstar.h>
#include <linux/i2c-dev.h>
#include <stdarg.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <IDGenerator.h>
#include <EmuGlobals.h>
#include <EmulatedHw.hpp>
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/loglevel.h>
#include <regex>

using namespace log4cplus;

namespace homerio {

int I2cBusEmulated::open(const char *file, int flag) {
  int fd;
  IDGenerator& g = IDGenerator::get_istance();
  Logger logemu = Logger::getInstance(LOGEMULATOR);
  LOG4CPLUS_TRACE(logemu,
                  "file=" << file << ",flag=" << flag << ",(" << fd << ")");
  fd = g.getId();
  filedescriptors[fd] = string(file);
  return (fd);
}
int I2cBusEmulated::close(int fd) {
  Logger logemu = Logger::getInstance(LOGEMULATOR);
  this->active_device = -1;
  LOG4CPLUS_TRACE(logemu, "fd=" << fd << ",file=" << filedescriptors[fd]);
  return (0);
}
int I2cBusEmulated::read(int fd, void *buf, size_t nbyte) {
  Logger logemu = Logger::getInstance(LOGEMULATOR);
  LOG4CPLUS_TRACE(
      logemu,
      "fd=" << fd << hex << ",buf=0x" << buf << ",nbyte=" << dec << nbyte
          << " file=" << filedescriptors[fd]);
  return (0);
}
int I2cBusEmulated::write(int fd, const void *buffer, size_t size) {
  Logger logemu = Logger::getInstance(LOGEMULATOR);
  uint8_t tmp[2] = { 0, 0 };
  if (size == 2) {
    tmp[0] = ((uint8_t *) buffer)[0];
    tmp[1] = ((uint8_t *) buffer)[1];
  }
  LOG4CPLUS_TRACE(
      logemu,
      "fd=" << fd << hex << ",buf=0x" << buffer << " [0x"
          << (unsigned int) tmp[0] << "," << (unsigned int) tmp[1] << "],size="
          << dec << size << " file=" << filedescriptors[fd]);
  if (write_obs_map.find(active_device) != write_obs_map.end()) {
    write_obs_map[active_device](fd, buffer, size);
  }
  return (size);
}
int I2cBusEmulated::ioctl(int fd, unsigned long int request, ...) {
  va_list arguments;
  va_start(arguments, request);
  Logger logemu = Logger::getInstance(LOGEMULATOR);

  switch (request) {
    case I2C_SLAVE:
      this->active_device = va_arg(arguments,unsigned int);
      break;
    default:
      return (-1);
  }
  LOG4CPLUS_TRACE(logemu, "fd=" << fd << ",file=" << filedescriptors[fd]);
  return (0);
}

__off_t I2cBusEmulated::lseek(int fd, __off_t                                                  __offset, int __whence) {
  Logger logemu = Logger::getInstance(LOGEMULATOR);
  LOG4CPLUS_TRACE(logemu, "fd=" << fd << ",file=" << filedescriptors[fd]);
  return (0);
}

int SysFsEmulated::open(const char *file, int flag) {
  int fd;
  IDGenerator& g = IDGenerator::get_istance();
  Logger logemu = Logger::getInstance(LOGEMULATOR);
  LOG4CPLUS_TRACE(logemu,
                  "file=" << file << ",flag=" << flag << ",(" << fd << ")");
  fd = g.getId();
  filedescriptors[fd] = string(file);
  return (fd);
}
int SysFsEmulated::close(int fd) {
  Logger logemu = Logger::getInstance(LOGEMULATOR);
  this->active_device = -1;
  LOG4CPLUS_TRACE(logemu, "fd=" << fd << ",file=" << filedescriptors[fd]);
  return (0);
}
int SysFsEmulated::read(int fd, void *buf, size_t nbyte) {
  int ret = -999;
  Logger logemu = Logger::getInstance(LOGEMULATOR);
  for (auto obs : read_obs_map) {
    regex rgx(obs.first);
    if (regex_match(filedescriptors[fd], rgx)) {
      obs.second(fd, buf, nbyte, filedescriptors[fd].c_str(), &ret);
    }
  }
  LOG4CPLUS_DEBUG(
      logemu,
      "fd=" << fd << hex << ",buf=0x" << buf << ",nbyte=" << dec << nbyte
          << " file=" << filedescriptors[fd] << " ret=" << ret);
  return (ret);
}

int SysFsEmulated::write(int fd, const void *buffer, size_t size) {
  int ret = -999;
  Logger logemu = Logger::getInstance(LOGEMULATOR);
  for (auto obs : write_obs_map) {
    regex rgx(obs.first);
    if (regex_match(filedescriptors[fd], rgx)) {
      obs.second(fd, buffer, size, filedescriptors[fd].c_str(), &ret);
    }
  }
  LOG4CPLUS_DEBUG(
      logemu,
      "fd=" << fd << hex << ",buffer=0x" << buffer << ",size=" << dec << size
          << " file=" << filedescriptors[fd] << " ret=" << ret);
  return (ret);

}
int SysFsEmulated::ioctl(int fd, unsigned long int request, ...) {
  return (-1);
}

__off_t SysFsEmulated::lseek(int fd, __off_t               __offset, int __whence) {
  Logger logemu = Logger::getInstance(LOGEMULATOR);
  LOG4CPLUS_TRACE(logemu, "fd=" << fd << ",file=" << filedescriptors[fd]);
  return (0);
}

int GpioPortEmulated::open(const char *file, int flag) {
  return (100);
}
int GpioPortEmulated::close(int fd) {
  return (0);
}
int GpioPortEmulated::read(int fd, void *buf, size_t nbyte) {
  return (0);
}
int GpioPortEmulated::write(int fd, const void *buffer, size_t size) {
  write_obs(fd, buffer, size);
  return (size);
}
int GpioPortEmulated::ioctl(int fd, unsigned long int request, ...) {
//	int ret = 0;
//	cerr << "GpioPort ioctl" << "req=" << hex << request;
//
//	switch(request) {
//	default:
//		cerr << "(UNKNOW)";
//		ret = -1;
//		break;
//	}
//	cerr << endl;
  return (0);
}
__off_t GpioPortEmulated::lseek(int fd, __off_t                                                  __offset, int __whence) {
  return (0);
}

}
