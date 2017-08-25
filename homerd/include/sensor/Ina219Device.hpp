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

#ifndef INA219DEVICE_HPP_
#define INA219DEVICE_HPP_
#include <CurrentDevice.hpp>
#include <VoltageDevice.hpp>
#include <PowerDevice.hpp>
#include <string.h>
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/loglevel.h>
#include <fstream>
#include <HwLayer.hpp>

#define INA219_ "/class/i2c-adapter/i2c-0/0-0040/hwmon/hwmon0/"
#define INA219_CURRENT INA219_ "curr1_input"
#define INA219_RSENSE_VOLTS INA219_ "in0_input"
#define INA219_VOLTAGE INA219_ "in1_input"
#define INA219_POWER INA219_ "power1_input"
#define INA219_SHUNT INA219_ "shunt_resistor"

#define BUFSIZE 50
using namespace std;

namespace homerio {

class Ina219Device {
 private:
  SysFs& sysFs;
  Logger _logdev;
  double shunt_micro_ohms;
  double shunt_scale_factor;

 public:
  Ina219Device(Board& _board)
      : sysFs(_board.getSysFs()),
        _logdev(Logger::getInstance(LOGDEVICE)) {
    shunt_micro_ohms = (double) readSysFsInteger(INA219_SHUNT);
    shunt_scale_factor = 10.0;
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

  virtual ~Ina219Device() {
  }

  double getShuntMicroOhms() const {
    return shunt_micro_ohms;
  }

  double getShuntScaleFactor() const {
    return shunt_scale_factor;
  }

  void setShuntScaleFactor(double shuntScaleFactor) {
    shunt_scale_factor = shuntScaleFactor;
  }
};

class Ina219Current : public CurrentDevice {
  Ina219Device& ina219Device;
  Logger _logdev;

 public:
  Ina219Current(Ina219Device& _ina219Device)
      : ina219Device(_ina219Device),
        _logdev(Logger::getInstance(LOGDEVICE)) {
  }
  void update(chrono::system_clock::time_point time_point) {
    if (update_point == time_point)
      return;
    update_point = time_point;
    amperes = (ina219Device.readSysFsInteger(INA219_CURRENT)
        / ina219Device.getShuntScaleFactor()) / 1000.0;
  }
}
;

class Ina219Voltage : public VoltageDevice {
  Ina219Device& ina219Device;
  Logger _logdev;
 public:
  Ina219Voltage(Ina219Device& _ina219Device)
      : ina219Device(_ina219Device),
        _logdev(Logger::getInstance(LOGDEVICE)) {
  }
  virtual ~Ina219Voltage() {
  }

  void update(chrono::system_clock::time_point time_point) {
    if (update_point == time_point)
      return;
    update_point = time_point;
    volts = ina219Device.readSysFsInteger(INA219_VOLTAGE) / 1000.0;
  }
}
;

class Ina219Rsens : public VoltageDevice {
  Ina219Device& ina219Device;
  Logger _logdev;
 public:
  Ina219Rsens(Ina219Device& _ina219Device)
      : ina219Device(_ina219Device),
        _logdev(Logger::getInstance(LOGDEVICE)) {
  }
  virtual ~Ina219Rsens() {
  }

  void update(chrono::system_clock::time_point time_point) {
    if (update_point == time_point)
      return;
    update_point = time_point;
    volts = ina219Device.readSysFsInteger(INA219_RSENSE_VOLTS) / 1000.0;
  }
}
;
class Ina219Power : public PowerDevice {
  Ina219Device& ina219Device;
  Logger _logdev;
 public:
  Ina219Power(Ina219Device& _ina219Device)
      : ina219Device(_ina219Device),
        _logdev(Logger::getInstance(LOGDEVICE)) {
  }
  virtual ~Ina219Power() {
  }

  void update(chrono::system_clock::time_point time_point) {
    if (update_point == time_point)
      return;
    update_point = time_point;
    watt = ((ina219Device.readSysFsInteger(INA219_POWER) / (10000.0))) / 1000.0;
  }
};

}

#endif /* INA219DEVICE_HPP_ */
