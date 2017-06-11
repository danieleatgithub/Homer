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

/*
 * DEV="/sys/class/i2c-adapter/i2c-0/0-0040/hwmon/hwmon0"
 # curr1_input :: current through Rsens
 I=$(cat $DEV/curr1_input)
 I_RSENSE=$(echo "scale=2;$I/1000" | bc)
 # in0_input :: voltage across Rsens
 V0=$(cat $DEV/in0_input)
 V_RSENSE=$(echo "scale=2;$V0/1000" | bc)
 # in1_input :: voltage from Bus to Gnd
 V1=$(cat $DEV/in1_input)
 V_BUS=$(echo "scale=2;$V1/1000" | bc)
 # power1_input:: power through Rsens
 P=$(cat $DEV/power1_input)
 P_RSENSE=$(echo "scale=2;$P/1000000" | bc)
 #echo "Rsens: $P_RSENSE mW - $V_RSENSE mV $I_RSENSE mA (Bus Voltage $V_BUS V) "
 echo "Current energy usage: $P_RSENSE mW $I_RSENSE mA $V_BUS V"
 */

#define INA219_CURRENT "/class/i2c-adapter/i2c-0/0-0040/hwmon/hwmon0/curr1_input"
#define INA219_RSENSE_VOLTS "/class/i2c-adapter/i2c-0/0-0040/hwmon/hwmon0/in0_input"
#define INA219_VOLTAGE "/class/i2c-adapter/i2c-0/0-0040/hwmon/hwmon0/in1_input"
#define INA219_POWER "/class/i2c-adapter/i2c-0/0-0040/hwmon/hwmon0/power1_input"
#define INA219_ "/class/i2c-adapter/i2c-0/0-0040/hwmon/hwmon0/"

#define BUFSIZE 50
using namespace std;

namespace homerio {

class Ina219Device {
 private:

  class Ina219Current : public CurrentDevice {
    char currentBuffer[100];
    SysFs& sysFs;
    Logger _logdev;
   public:
    Ina219Current(SysFs& sysFs)
        : sysFs(sysFs),
          _logdev(Logger::getInstance(LOGDEVICE)) {
    }
    // FIXME: test file not found
    void update() {
      int nread;
      nread = sysFs.readBuffer(INA219_CURRENT, currentBuffer, (BUFSIZE - 1));
      amperes = atoi(currentBuffer) / 1000000.0;
      LOG4CPLUS_TRACE(
          _logdev,
          "Read " << INA219_CURRENT << " string(" << nread << ") ["
              << currentBuffer << "]");
    }

  };
  class Ina219Voltage : public VoltageDevice {
    char voltageBuffer[100];
    SysFs& sysFs;
    Logger _logdev;
    const char *sysEntry;
   public:
    Ina219Voltage(SysFs& sysFs, const char *sysEntry)
        : sysFs(sysFs),
          _logdev(Logger::getInstance(LOGDEVICE)) {
      this->sysEntry = sysEntry;
    }
    // FIXME: test file not found
    void update() {
      int nread;
      nread = sysFs.readBuffer(sysEntry, voltageBuffer, (BUFSIZE - 1));
      volts = atoi(voltageBuffer) / 1000000.0;
      LOG4CPLUS_TRACE(
          _logdev,
          "Read " << sysEntry << " string(" << nread << ") [" << voltageBuffer
              << "]");

    }
  }
  ;
  class Ina219Power : public PowerDevice {
    char powerBuffer[100];
    SysFs& sysFs;
    Logger _logdev;
   public:
    Ina219Power(SysFs& sysFs)
        : sysFs(sysFs),
          _logdev(Logger::getInstance(LOGDEVICE)) {
    }
    // FIXME: test file not found
    void update() {
      int nread;
      nread = sysFs.readBuffer(INA219_POWER, powerBuffer, (BUFSIZE - 1));
      milliwatts = atoi(powerBuffer) / 1000000.0;
      LOG4CPLUS_TRACE(
          _logdev,
          "Read " << INA219_POWER << " string(" << nread << ") [" << powerBuffer
              << "]");

    }
  };

  SysFs& sysFs;
  Ina219Current current;
  Ina219Power power;
  Ina219Voltage voltage;
  Ina219Voltage rsensVolts;
  Logger _logdev;

 public:
  Ina219Device(Board& _board)
      : sysFs(_board.getSysFs()),
        current(_board.getSysFs()),
        power(_board.getSysFs()),
        voltage(_board.getSysFs(), INA219_VOLTAGE),
        rsensVolts(_board.getSysFs(), INA219_RSENSE_VOLTS),
        _logdev(Logger::getInstance(LOGDEVICE)) {
  }

  virtual ~Ina219Device() {
  }

  CurrentDevice& getCurrent() {
    return current;
  }

  PowerDevice& getPower() {
    return power;
  }

  VoltageDevice& getRsensVolts() {
    return rsensVolts;
  }

  VoltageDevice& getVoltage() {
    return voltage;
  }

};

}

#endif /* INA219DEVICE_HPP_ */
