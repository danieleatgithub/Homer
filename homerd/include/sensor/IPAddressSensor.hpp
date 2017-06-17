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

#ifndef IPADDRESSSENSOR_HPP_
#define IPADDRESSSENSOR_HPP_
#include <Sensor.hpp>
#include <Sysinfo.h>
#include <sstream>
#include <map>
#include <math.h>
#include <iomanip>

using namespace std;

namespace homerio {

class IPAddressSensor : public Sensor, public MenuAble {
 public:
  IPAddressSensor(string _interface, string _label)
      : Sensor(_label),
        phy_if(_interface),
        label(_label) {

  }
  ~IPAddressSensor() {
  }
  ;

  void update() {
    ip = Sysinfo::get_instance().get_local_ip(phy_if.c_str());
  }
  const string getValue() const {
    return ip;
  }
  const string getLabel() const {
    return label;
  }
  const string getString() const {
    return ip;
  }
  const double getDouble() const {
    return 0.0;
  }
 private:
  string phy_if;
  string label;
  string ip;

};
}

#endif /* TEMPERATURESENSOR_HPP_ */
