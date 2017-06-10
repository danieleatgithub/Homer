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

#ifndef POWERSENSOR_HPP_
#define POWERSENSOR_HPP_
#include <Sensor.hpp>
#include <PowerDevice.hpp>

using namespace std;

namespace homerio {

class PowerSensor : public Sensor, public MenuAble {
 public:
  PowerSensor(PowerDevice& _device, string _label)
      : Sensor(_label),
        device(_device),
        label(_label) {

  }
  ~PowerSensor() {
  }
  ;
  PowerDevice& getDevice() const {
    return device;
  }

  double getWatts() const {
    return device.getWatts();
  }
  void update() {
    device.update();
  }
  const string getValue() const {
    ostringstream ostr;
    ostr << device.getWatts() << " W";
    return ostr.str();
  }
  const string getLabel() const {
    return label;
  }
  const string getString() const {
    ostringstream ostr;
    ostr << device.getWatts();
    return ostr.str();
  }
  const double getDouble() const {
    return device.getWatts();
  }
 private:
  PowerDevice& device;
  string label;

};
}

#endif /* POWERSENSOR_HPP_ */
