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

#ifndef HIH5030DEVICE_HPP_
#define HIH5030DEVICE_HPP_
#include <HumidityDevice.hpp>
#include <string.h>
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/loglevel.h>
#include <fstream>
#include <cmath>
#include <HwLayer.hpp>

#define HIH5030_ "/bus/iio/devices/iio:device1/"
#define HIH5030_RH (HIH5030_ "in_voltage0_raw")
#define HIH5030_SCALE (HIH5030_ "in_voltage_scale" )

#define BUFSIZE 50
using namespace std;

namespace homerio {

class Hih5030Device {
 private:
  SysFs& sysFs;
  Logger _logdev;

 public:
  Hih5030Device(Board& _board)
      : sysFs(_board.getSysFs()),
        _logdev(Logger::getInstance(LOGDEVICE)) {

  }

  int readSysFsInteger(const char *entry) {
    int nread = 0;
    int value = 0;
    char buffer[BUFSIZE];
    nread = sysFs.readBuffer(entry, buffer, (BUFSIZE - 1));
    if (nread > 0) {
      value = atoi(buffer);
      LOG4CPLUS_TRACE(
          _logdev,
          "Read " << entry << " string(" << nread << ") [" << buffer << "]");
    } else {
      LOG4CPLUS_ERROR(_logdev, "Read " << entry << " string(" << nread << ")");
    }
    return (value);
  }
  double readSysFsDouble(const char *entry) {
    int nread = 0;
    double value = 0;
    char buffer[BUFSIZE];
    nread = sysFs.readBuffer(entry, buffer, (BUFSIZE - 1));
    if (nread > 0) {
      value = strtod(buffer, NULL);
      LOG4CPLUS_TRACE(
          _logdev,
          "Read " << entry << " string(" << nread << ") [" << buffer
              << "] value=" << value);
    } else {
      LOG4CPLUS_ERROR(_logdev, "Read " << entry << " string(" << nread << ")");
    }
    return (value);
  }

  // Honeywell HIH-5030 Datasheet Table 1
  // https://sensing.honeywell.com/honeywell-sensing-hih5030-5031-series-product-sheet-009050-2-en.pdf
  //  Performance Specifications (At 3.3 Vdc supply and 25 C [77 F] unless otherwise noted.)
  double getRH(double celsius) {
    double raw_rh, rh1, rh, volt;
    double scale;
    raw_rh = readSysFsInteger(HIH5030_RH) * 1.0;
    scale = readSysFsDouble(HIH5030_SCALE);
    if (raw_rh == 0 || scale == 0)
      return (0);
    volt = (raw_rh * scale) / 1000;
    rh1 = ((volt / 3.1) - 0.1515) / 0.00636;
    rh = rh1 / (1.0546 - 0.00216 * celsius);
    LOG4CPLUS_TRACE(
        _logdev,
        "getRH raw_rh=" << raw_rh << "S=" << scale << " V=" << volt << " rh1="
            << rh1 << " T=" << celsius << " RH=" << rh);
    return (rh);
  }

  virtual ~Hih5030Device() {
  }

};

class Hih5030fHumidity : public HumidityDevice {
  Hih5030Device& hih5030Device;
  TemperatureDevice& temperatureDevice;
  Logger _logdev;

 public:
  Hih5030fHumidity(Hih5030Device& _hih5030Device,
                   TemperatureDevice& _temperatureDevice)
      : hih5030Device(_hih5030Device),
        temperatureDevice(_temperatureDevice),
        _logdev(Logger::getInstance(LOGDEVICE)) {
  }
  void update(chrono::system_clock::time_point time_point) {
    if (update_point == time_point)
      return;
    temperatureDevice.update(time_point);
    update_point = time_point;
    rh = hih5030Device.getRH(temperatureDevice.getCelsius());
  }
}
;

}

#endif /* HIH5030DEVICE_HPP_ */
