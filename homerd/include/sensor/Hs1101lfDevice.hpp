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

#ifndef HS1101LFDEVICE_HPP_
#define HS1101LFDEVICE_HPP_
#include <HumidityDevice.hpp>
#include <string.h>
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/loglevel.h>
#include <fstream>
#include <cmath>
#include <HwLayer.hpp>

#define HS1101LF_ "/bus/iio/devices/iio:device2/"
#define HS1101LF_RH (HS1101LF_ "/in_humidityrelative_raw")
#define HS1101LF_CYCLES (HS1101LF_ "/cycles" )
#define HS1101LF_FREQUENCY (HS1101LF_ "/frequency" )
#define HS1101LF_SAMPLE_MS (HS1101LF_ "/sample_ms" )

#define BUFSIZE 50
using namespace std;

namespace homerio {

class Hs1101lfDevice {
 private:
  SysFs& sysFs;
  Logger _logdev;
  int cycles;
  int frequency;
  int sample_ms;

 public:
  Hs1101lfDevice(Board& _board)
      : sysFs(_board.getSysFs()),
        _logdev(Logger::getInstance(LOGDEVICE)) {
    sample_ms = (double) readSysFsInteger(HS1101LF_SAMPLE_MS);

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
  double getRH(double celsius) {
    double rh, rh1;
    rh1 = readSysFsInteger(HS1101LF_RH) / 100.0;
    cycles = readSysFsInteger(HS1101LF_CYCLES);
    frequency = readSysFsInteger(HS1101LF_FREQUENCY);
    if (rh1 == 0 || cycles == 0)
      return (0);

    rh = rh1 / (1.0546 - 0.00216 * celsius);
    LOG4CPLUS_DEBUG(
        _logdev,
        "getRH c=" << cycles << " f=" << frequency << " rh1=" << rh1 << " T="
            << celsius << " RH=" << rh);
    return (rh);
  }

  virtual ~Hs1101lfDevice() {
  }

};

class Hs1101lfHumidity : public HumidityDevice {
  Hs1101lfDevice& hs1101lfDevice;
  TemperatureDevice& temperatureDevice;
  Logger _logdev;

 public:
  Hs1101lfHumidity(Hs1101lfDevice& _hs1101lfDevice,
                   TemperatureDevice& _temperatureDevice)
      : hs1101lfDevice(_hs1101lfDevice),
        temperatureDevice(_temperatureDevice),
        _logdev(Logger::getInstance(LOGDEVICE)) {
  }
  void update(chrono::system_clock::time_point time_point) {
    if (update_point == time_point)
      return;
    temperatureDevice.update(time_point);
    update_point = time_point;
    rh = hs1101lfDevice.getRH(temperatureDevice.getCelsius());
  }
}
;

}

#endif /* HS1101LFDEVICE_HPP_ */
