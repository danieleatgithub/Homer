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

#ifndef GPIOPIN_H_
#define GPIOPIN_H_

#include <string>
#include <HwLayer.hpp>

using namespace std;

#define SYSFS_GPIO_DIR "/sys/class/gpio"
#define MAX_BUF 64

namespace homerio {

enum State_e {
  STATE_OFF = 0,
  STATE_ON,
  STATE_TOGGLE
};
enum Direction_e {
  OUT = 0,
  INP
};

enum Edge_e {
  RISING = 0,
  FALLING,
  NONE,
  BOTH
};

class GpioPin {
 private:
  string kstr;
  int kid;
  int fd;
  bool in_use;
  string name;
  Edge_e edge;
  Direction_e direction;
  bool last_value;
  int write_value(bool value);
  GpioPort& gpioPort;

 public:
  static struct pinmap_s* getPinDescriptor(const char *name);
  GpioPin(GpioPort& port);

  virtual ~GpioPin();
  int get_pin();
  int pin_export();
  int pin_unexport();
  int set_direction(Direction_e dir);
  int set_edge(Edge_e edge);
  int setState(State_e state);
  int flip(unsigned int us);
  int get(bool *value);
  int pin_open();
  int pin_close();
  int getfd();

};

}

#endif /* GPIOPIN_H_ */
