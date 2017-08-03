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

#ifndef CURRENTDEVICE_HPP_
#define CURRENTDEVICE_HPP_

using namespace std;

namespace homerio {

class CurrentDevice {
 protected:
  double amperes;
  chrono::system_clock::time_point update_point;
 public:
  CurrentDevice() {
    amperes = 0;
  }
  virtual ~CurrentDevice() {
  }
  ;

  double getAmperes() const {
    return amperes;
  }
  virtual void update(chrono::system_clock::time_point time_point) = 0;
};

}
#endif /* CURRENTDEVICE_HPP_ */
