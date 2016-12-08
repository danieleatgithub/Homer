/*
 * KeyPanel.cpp
 *
 *  Created on: 08/dic/2016
 *      Author: daniele
 */

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
