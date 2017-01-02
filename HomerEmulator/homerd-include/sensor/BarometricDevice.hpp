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

using namespace std;

namespace homerio {

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

}

#endif /* BAROMETRICDEVICE_HPP_ */
