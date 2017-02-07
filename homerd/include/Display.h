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
#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>
#include <string>
#include "KeyPanel.hpp"
#include "Scheduler.hpp"
#include <HwLayer.hpp>

using namespace std;
using namespace commodities;

namespace homerio {
class GpioPin;

class Display {

 private:
  unsigned int key_light_delay;
  unsigned int light_remain_ms;
  Task timedLightOff;
  KeyPanel& keyPanel;
  Scheduler& scheduler;
  obs::Registration keyPanel_reg;

 protected:
  uint8_t address;
  string bus;
  string rst;
  int write_usleep;
  I2cBus& i2cBus;

  string backlight;
  GpioPin* backlight_pin;
  GpioPin* reset_pin;
  bool backlight_state;
  virtual int device_init() = 0;

 public:

  Display(KeyPanel &key_panel, Scheduler &scheduler, I2cBus& i2cBus,
          GpioPort& rst, GpioPort& backlight);
  virtual ~Display();

  int dpy_putchar(unsigned char ch);
  int dpy_puts(const char *str);
  int set_backlight(bool state);
  bool is_backlight_on();

  virtual int reset() = 0;
  virtual int set_state(bool state) = 0;
  virtual bool is_display_on() = 0;
  virtual bool is_cursor_on() = 0;
  virtual int get_contrast() = 0;
  virtual int set_contrast(int value) = 0;
  virtual int set_cursor_on(bool state) = 0;
  virtual int set_cursor_blink(bool state) = 0;

  virtual int set_two_lines() = 0;
  virtual int set_one_line() = 0;
  virtual int set_double_height() = 0;
  virtual bool is_two_lines() = 0;
  virtual int clear() = 0;
  virtual int home() = 0;
  virtual int line2_home() = 0;
  virtual int shift_line() = 0;
  virtual int shift_cursor() = 0;
  virtual int set_insert_mode() = 0;
  virtual int set_overwrite_mode() = 0;
  virtual int write_data(unsigned char data) = 0;
};
}
#endif
