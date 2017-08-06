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

#ifndef BMP085DEVICE_HPP_
#define BMP085DEVICE_HPP_
#include <TemperatureDevice.hpp>
#include <BarometricDevice.hpp>
#include <string.h>
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/loglevel.h>
#include <fstream>
#include <HwLayer.hpp>

#define BMP085_TEMPERATURE "/class/i2c-adapter/i2c-0/0-0077/temp0_input"
#define BMP085_PRESSURE "/class/i2c-adapter/i2c-0/0-0077/pressure0_input"
#define BUFSIZE 50

using namespace std;

namespace homerio {

class Bmp085Device {
 private:
  SysFs& sysFs;
  Logger _logdev;

 public:

  Bmp085Device(Board& _board)
      : sysFs(_board.getSysFs()),
        _logdev(Logger::getInstance(LOGDEVICE)) {
  }

  virtual ~Bmp085Device() {
  }

  long readSysFsLong(const char *entry) {
    int nread = 0;
    long value = 0;
    char buffer[BUFSIZE];
    nread = sysFs.readBuffer(entry, buffer, (BUFSIZE - 1));
    if (nread > 0) {
      value = atol(buffer);
      LOG4CPLUS_TRACE(
          _logdev,
          "Read " << entry << " string(" << nread << ") [" << buffer << "]");
    } else {
      LOG4CPLUS_ERROR(_logdev, "Read " << entry << " string(" << nread << ")");
    }
    return (value);
  }

};

class Bmp085Thermometer : public TemperatureDevice {
 private:
  Bmp085Device& bmp085Device;
  Logger _logdev;

 public:
  Bmp085Thermometer(Bmp085Device& _bmp085Device)
      : bmp085Device(_bmp085Device),
        _logdev(Logger::getInstance(LOGDEVICE)) {
  }

  void update(chrono::system_clock::time_point time_point) {
    if (update_point == time_point)
      return;
    update_point = time_point;
    celsius = bmp085Device.readSysFsLong(BMP085_TEMPERATURE) / 10.0;
  }

};

class Bmp085Barometer : public BarometricDevice {
 private:
  Bmp085Device& bmp085Device;
  Logger _logdev;

 public:
  Bmp085Barometer(Bmp085Device& _bmp085Device)
      : bmp085Device(_bmp085Device),
        _logdev(Logger::getInstance(LOGDEVICE)) {
  }
  void update(chrono::system_clock::time_point time_point) {
    if (update_point == time_point)
      return;
    update_point = time_point;
    millibar = bmp085Device.readSysFsLong(BMP085_PRESSURE) / 100.0;
  }
}
;

}

#endif /* BMP085DEVICE_HPP_ */
