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

#ifndef KEYBUTTON_HPP_
#define KEYBUTTON_HPP_

#include <chrono>
#include <thread>
#include <string>
#include "Observer.h"
#include <linux/input.h>
#include <map>

using namespace std;

namespace homerio {

/**
 * All keybutton available in homer
 */
enum Button_e {
  BUTTON_ENTER = KEY_ENTER,  //!< BUTTON_ENTER
  BUTTON_UP = KEY_UP,      //!< BUTTON_UP
  BUTTON_DOWN = KEY_DOWN,  //!< BUTTON_DOWN
  BUTTON_LEFT = KEY_LEFT,  //!< BUTTON_LEFT
  BUTTON_RIGHT = KEY_RIGHT,  //!< BUTTON_RIGHT
  BUTTON_NULL = KEY_RESERVED  //!< no button
};

static std::map<__u16, string> keycode2string = {
    { BUTTON_ENTER, "BUTTON_ENTER" }, { BUTTON_UP, "BUTTON_UP" }, { BUTTON_DOWN,
        "BUTTON_DOWN" }, { BUTTON_LEFT, "BUTTON_LEFT" }, { BUTTON_RIGHT,
        "BUTTON_RIGHT" }, { BUTTON_NULL, "BUTTON_NULL" } };
;

class KeyButton {
 private:
  struct input_event key;
  struct timeval t0;
  struct timeval t1;
  std::chrono::milliseconds pressTime;
  std::chrono::milliseconds longThreshold;
  bool pressed;
  bool valid;

 public:
  KeyButton();
  void loadEvent(struct input_event ev);
  void validateEvent(struct input_event ev);

  const bool isPressEvent();
  const bool isLongEvent();
  const Button_e getKey();
  const std::chrono::milliseconds getPresstime();
  const std::chrono::milliseconds& getLongThreshold() const;
  void setLongThreshold(const std::chrono::milliseconds& _longThreshold);
  operator std::string() const;

};

}

#endif /* KEYPANEL_HPP_ */
