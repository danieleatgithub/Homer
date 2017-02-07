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

#include <KeyButton.hpp>
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/loglevel.h>
#include "homerd.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>

using namespace std;
using namespace log4cplus;

namespace homerio {

KeyButton::KeyButton() {
  valid = false;
  pressed = false;
  longThreshold = std::chrono::milliseconds(1000);
  key.code = BUTTON_NULL;
  key.value = 0;
  pressTime = std::chrono::milliseconds(0);
}

void KeyButton::loadEvent(struct input_event ev) {
  key = ev;
  if (ev.value == 1) {
    t0 = ev.time;
    pressed = true;
  } else {
    pressed = false;
    t1 = ev.time;
  }
  valid = false;
}

void KeyButton::validateEvent(struct input_event ev) {
  valid = true;
  // SYN on button release
  if (ev.value == 0 && !pressed) {
    pressTime = std::chrono::milliseconds(t1.tv_usec - t0.tv_usec) / 1000
        + std::chrono::milliseconds(t1.tv_sec - t0.tv_sec) * 1000;
  }
}

KeyButton::operator std::string() const {
  return (string("Key=") + std::to_string(key.code) + string(" (")
      + keycode2string[key.code]
      + string(pressed ? ") PRESSED ms=" : ") RELEASED ms=")
      + std::to_string(this->pressTime.count()) + string(" isLong=")
      + string(this->pressTime >= longThreshold ? "Y" : "N"));
}
const bool KeyButton::isPressEvent() {
  return (key.value == 1);
}
const bool KeyButton::isLongEvent() {
  return (this->pressTime >= longThreshold);
}
const Button_e KeyButton::getKey() {
  return ((Button_e) key.code);
}
const std::chrono::milliseconds KeyButton::getPresstime() {
  return (pressTime);
}

const std::chrono::milliseconds& KeyButton::getLongThreshold() const {
  return longThreshold;
}

void KeyButton::setLongThreshold(
    const std::chrono::milliseconds& _longThreshold) {
  longThreshold = _longThreshold;
}

}
