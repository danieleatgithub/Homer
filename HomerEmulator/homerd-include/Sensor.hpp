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

#ifndef SENSOR_HPP_
#define SENSOR_HPP_
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/loglevel.h>
#include <chrono>
#include <string>
#include <iostream>
#include <fstream>
#include <HomerMenu.hpp>

#define BMP085_TEMPERATURE "/class/i2c-adapter/i2c-0/0-0077/temp0_input"
#define BMP085_PRESSURE "/class/i2c-adapter/i2c-0/0-0077/pressure0_input"

using namespace std;

namespace homerio {

/*
 *
 *
 *
 *  devices
 *
 *
 */

class TemperatureDevice {
 public:
  double temperature;
  TemperatureDevice() {
    temperature = 0;
  }
  virtual ~TemperatureDevice() {
  }
  ;
  virtual void readTemperature() = 0;
  double getTemperature() const {
    return temperature;
  }
  void update() {
    readTemperature();
  }
};

class BarometricDevice {
 public:
  double pressure;
  double localAltitude;

  BarometricDevice() {
    pressure = 0;
    localAltitude = 0;
  }
  virtual ~BarometricDevice() {
  }
  ;
  virtual void readPressure() = 0;

  double getAltituteCalibration() const {
    return localAltitude;
  }
  void setAltituteCalibration(double localAltitude) {
    this->localAltitude = localAltitude;
  }
  /*
   LOCALPA=$(echo "scale=2;$ALTITUDE/100*12" | bc)
   P=$(cat $DEV/pressure0_input)
   PRESSURE=$(echo "scale=2;$P/100+$LOCALPA" | bc)
   echo "Temp=$TEMP Pressure=$PRESSURE"

   [root@homer ~]# bash -x ./bmp085.sh
   + modprobe bmp085_i2c
   + ALTITUDE=354
   + DEV=/sys/class/i2c-adapter/i2c-0/0-0077
   ++ cat /sys/class/i2c-adapter/i2c-0/0-0077/temp0_input
   + T=205
   ++ bc
   ++ echo 'scale=2;205/10'
   + TEMP=20.50
   ++ echo 'scale=2;354/100*12'
   ++ bc
   + LOCALPA=42.48
   ++ cat /sys/class/i2c-adapter/i2c-0/0-0077/pressure0_input
   + P=99614
   ++ bc
   ++ echo 'scale=2;99614/100+42.48'
   + PRESSURE=1038.628
   + echo 'Temp=20.50 Pressure=1038.62'
   Temp=20.50 Pressure=1038.62

   */

  double getPressure() const {
    // Add 12 mBar each 100mt of altitude
    return pressure + (localAltitude / 100.0) * 12.0;
  }
  void update() {
    readPressure();
  }
};

class Bmp085Device : public TemperatureDevice, public BarometricDevice {
 private:
  string sysPath;
  SysFs& sysFs;
  char temperatureBuffer[100];
  char pressureBuffer[100];

 public:
  Bmp085Device(Board& _board)
      : sysFs(_board.getSysFs()) {
  }

  virtual ~Bmp085Device() {

  }
// FIXME: test file not found
  void readTemperature() {
    int nread;
    Logger logdev = Logger::getInstance(LOGDEVICE);
    nread = sysFs.readBuffer(BMP085_TEMPERATURE, temperatureBuffer,
                             (sizeof(temperatureBuffer) - 1));
    temperature = atoi(temperatureBuffer) / 10.0;
    LOG4CPLUS_TRACE(
        logdev,
        "Read " << BMP085_TEMPERATURE << " string(" << nread << ") ["
            << temperatureBuffer << "]");
  }
  void readPressure() {
    int nread;
    Logger logdev = Logger::getInstance(LOGDEVICE);
    nread = sysFs.readBuffer(BMP085_PRESSURE, pressureBuffer,
                             (sizeof(pressureBuffer) - 1));

    pressure = atol(pressureBuffer) / 100.0;
    LOG4CPLUS_TRACE(
        logdev,
        "Read " << BMP085_PRESSURE << " string(" << nread << ") ["
            << pressureBuffer << "]");
  }

  const string& getSysPath() const {
    return sysPath;
  }

};
/*
 *
 *
 * Sensor
 *
 *
 *
 */

class Sensor : public MenuAble {

 public:

  Sensor()
      : MenuAble() {
  }
  virtual ~Sensor() {
  }

};

class TemperatureSensor : public Sensor {
 public:
  TemperatureSensor(TemperatureDevice& _device, string _label)
      : Sensor(),
        device(_device),
        label(_label) {

  }
  ~TemperatureSensor() {
  }
  ;
  TemperatureDevice& getDevice() const {
    return device;
  }

  double getTemperature() const {
    return device.getTemperature();
  }

  void update() {
    device.update();
  }
  const string getValue() const {
    ostringstream ostr;
    ostr << device.getTemperature() << " C";
    return ostr.str();
  }
  const string getLabel() const {
    return label;
  }
 private:
  TemperatureDevice& device;
  string label;

};

class BarometricSensor : public Sensor {
 public:
  BarometricSensor(BarometricDevice& _device, string _label)
      : Sensor(),
        device(_device),
        label(_label) {

  }
  ~BarometricSensor() {
  }
  ;
  BarometricDevice& getDevice() const {
    return device;
  }

  double getPressure() const {
    return device.getPressure();
  }
  const string getValue() const {
    ostringstream ostr;
    ostr << device.getPressure() << " mBar";
    return ostr.str();
  }
  const double getAltitudeCalibration() const {
    return (device.getAltituteCalibration());
  }
  void setAltituteCalibration(double localAltitude) {
    device.setAltituteCalibration(localAltitude);
  }
  void update() {
    device.update();
  }
  const string getLabel() const {
    return label;
  }
 private:
  BarometricDevice& device;
  string label;
};

}

#endif /* SENSOR_HPP_ */
