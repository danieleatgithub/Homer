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

#ifndef BAROMETRICDEVICE_HPP_
#define BAROMETRICDEVICE_HPP_

#define MILLIBAR_METER 0.12 // Millibar each meter good aproximation under 1000 mt

using namespace std;

namespace homerio {

class BarometricDevice {
 protected:
  double millibar;
  double localAltitude;

 public:

  BarometricDevice() {
    millibar = 0;
    localAltitude = 0;
  }
  virtual ~BarometricDevice() {
  }
  ;

  double getAltituteCalibration() const {
    return localAltitude;
  }
  void setAltituteCalibration(double localAltitude) {
    this->localAltitude = localAltitude;
  }
  /*
   [root@homer ~]# bash -x ./bmp085.sh
   + modprobe bmp085_i2c
   + ALTITUDE=330
   + DEV=/sys/class/i2c-adapter/i2c-0/0-0077
   ++ cat /sys/class/i2c-adapter/i2c-0/0-0077/temp0_input
   + T=216
   ++ bc
   ++ echo 'scale=2;216/10'
   + TEMP=21.60
   ++ bc
   ++ echo 'scale=2;330/100*12'
   + LOCALPA=39.60
   ++ cat /sys/class/i2c-adapter/i2c-0/0-0077/pressure0_input
   + P=97052
   ++ bc
   ++ echo 'scale=2;97052/100+39.60'
   + PRESSURE=1010.12
   + echo 'Temp=21.60 Pressure=1010.12'
   Temp=21.60 Pressure=1010.12

   */

  double getMilliBar() const {
    // Add 12 mBar each 100mt of altitude
    return millibar + (localAltitude * MILLIBAR_METER);
  }
  virtual void update() = 0;
};

}

#endif /* BAROMETRICDEVICE_HPP_ */
