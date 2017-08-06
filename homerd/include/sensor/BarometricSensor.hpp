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

#ifndef BAROMETRICSENSOR_HPP_
#define BAROMETRICSENSOR_HPP_
#include <Sensor.hpp>
#include <BarometricDevice.hpp>
#include <sstream>
#include <map>
#include <math.h>
#include <iomanip>

using namespace std;

namespace homerio {

class BarometricSensor : public Sensor, public MenuAble {
 public:
  BarometricSensor(BarometricDevice& _device, string _label)
      : Sensor(_label),
        device(_device),
        label(_label) {
    units[0] = "milli";
    units[3] = "";
    scale = 0;
    precision = 6;
  }
  ~BarometricSensor() {
  }
  ;
  BarometricDevice& getDevice() const {
    return device;
  }

  double getMilliBar() const {
    return device.getMilliBar();
  }
  const double getAltitudeCalibration() const {
    return (device.getAltituteCalibration());
  }
  void setAltituteCalibration(double localAltitude) {
    device.setAltituteCalibration(localAltitude);
  }
  const string getValue() const {
    ostringstream ostr;
    ostr << std::setprecision(precision)
        << (device.getMilliBar() / pow(10.0, scale)) << " "
        << units.find(scale)->second << "Bar";
    return ostr.str();
  }
  void update(chrono::system_clock::time_point time_point) {
    device.update(time_point);
  }
  void refresh(chrono::system_clock::time_point time_point) {
    update(time_point);
  }
  const string getLabel() const {
    return label;
  }
  const string getString() const {
    ostringstream ostr;
    ostr << device.getMilliBar();
    return ostr.str();
  }
  const double getDouble() const {
    return device.getMilliBar();
  }
 private:
  BarometricDevice& device;
  string label;
  static map<int, string> _barometricUnits;

};

}

#endif /* BAROMETRICSENSOR_HPP_ */
