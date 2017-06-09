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

using namespace std;

namespace homerio {

class Bmp085Device : public CurrentDevice, public BarometricDevice {
 private:
  string sysPath;
  SysFs& sysFs;
  char temperatureBuffer[100];
  char pressureBuffer[100];
  Logger _logdev;

 public:
  Bmp085Device(Board& _board)
      : sysFs(_board.getSysFs()),
        _logdev(Logger::getInstance(LOGDEVICE)) {
  }

  virtual ~Bmp085Device() {

  }
// FIXME: test file not found
  void readTemperature() {
    int nread;
    nread = sysFs.readBuffer(BMP085_TEMPERATURE, temperatureBuffer,
                             (sizeof(temperatureBuffer) - 1));
    temperature = atoi(temperatureBuffer) / 10.0;
    LOG4CPLUS_TRACE(
        _logdev,
        "Read " << BMP085_TEMPERATURE << " string(" << nread << ") ["
            << temperatureBuffer << "]");
  }
  void readPressure() {
    int nread;
    nread = sysFs.readBuffer(BMP085_PRESSURE, pressureBuffer,
                             (sizeof(pressureBuffer) - 1));

    millibar = atol(pressureBuffer) / 100.0;
    LOG4CPLUS_TRACE(
        _logdev,
        "Read " << BMP085_PRESSURE << " string(" << nread << ") ["
            << pressureBuffer << "]");
  }

  const string& getSysPath() const {
    return sysPath;
  }

};

}

#endif /* BMP085DEVICE_HPP_ */
