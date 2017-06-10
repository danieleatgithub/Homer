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

#ifndef CURRENTSENSOR_HPP_
#define CURRENTSENSOR_HPP_
#include <Sensor.hpp>
#include <CurrentDevice.hpp>

using namespace std;

namespace homerio {

class CurrentSensor : public Sensor, public MenuAble {
 public:
  CurrentSensor(CurrentDevice& _device, string _label)
      : Sensor(_label),
        device(_device),
        label(_label) {

  }
  ~CurrentSensor() {
  }
  ;
  CurrentDevice& getDevice() const {
    return device;
  }

  double getVolts() const {
    return device.getAmperes();
  }
  void update() {
    device.update();
  }
  const string getValue() const {
    ostringstream ostr;
    ostr << device.getAmperes() << " A";
    return ostr.str();
  }
  const string getLabel() const {
    return label;
  }
  const string getString() const {
    ostringstream ostr;
    ostr << device.getAmperes();
    return ostr.str();
  }
  const double getDouble() const {
    return device.getAmperes();
  }
 private:
  CurrentDevice& device;
  string label;

};
}

#endif /* VOLTAGESENSOR_HPP_ */
