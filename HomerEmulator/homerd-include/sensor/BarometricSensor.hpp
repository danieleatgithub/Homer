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
using namespace std;

namespace homerio {

class BarometricSensor : public Sensor, public MenuAble {
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
  const double getNumeric() const {
    return device.getPressure();
  }
  const double getAltitudeCalibration() const {
    return (device.getAltituteCalibration());
  }
  void setAltituteCalibration(double localAltitude) {
    device.setAltituteCalibration(localAltitude);
  }
  const string getValue() const {
    ostringstream ostr;
    ostr << device.getPressure() << " mBar";
    return ostr.str();
  }
  void update() {
    device.update();
  }
  const string getLabel() const {
    return label;
  }
  const string get() const {
    ostringstream ostr;
    ostr << device.getPressure() << " mBar";
    return ostr.str();
  }

 private:
  BarometricDevice& device;
  string label;
};

}

#endif /* BAROMETRICSENSOR_HPP_ */
