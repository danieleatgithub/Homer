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

#ifndef WINSTAR_H_
#define WINSTAR_H_

#include "Display.h"
#include "GpioPin.h"

namespace homerio {

#define WINSTAR_I2C_ADD                 0x3e
#define WSTAR_CMD						0
#define WSTAR_DATA						0x40

#define WSTAR_CLEAR_DISPLAY_CMD             0x01
#define WSTAR_RETURN_HOME_CMD               0x02

#define WSTAR_ENTRY_MODE_SHIFT_ON			0x01
#define WSTAR_ENTRY_MODE_RIGHT				0x02
#define WSTAR_ENTRY_MODE_CMD				0x04

#define WSTAR_DISPLAY_BLINK					0x01
#define WSTAR_DISPLAY_CURSOR				0x02
#define WSTAR_DISPLAY_STATE					0x04
#define WSTAR_DISPLAY_CMD					0x08

#define WSTAR_CUR_DPY_SHIFT_RIGHT			0x04
#define WSTAR_CUR_DPY_SHIFT_SCREEN			0x08
#define WSTAR_CUR_DPY_SHIFT_CMD				0x10

#define WSTAR_FUNCTION_EXTENDED				0x01
#define WSTAR_FUNCTION_ONE_LINE_5X8_MASK	0x0C	// Reset bitmap
#define WSTAR_FUNCTION_ONE_LINE_5X16		0x04
#define WSTAR_FUNCTION_TWO_LINE_5X8			0x08
#define WSTAR_FUNCTION_8BIT					0x10
#define WSTAR_FUNCTION_CMD					0x20

#define WSTAR_CGRAM_ADD_MASK				0x3F
#define WSTAR_CGRAM_CMD						0x40

#define WSTAR_DDRAM_ADD_MASK				0x7F
#define WSTAR_DDRAM_CMD						0x80
#define WSTAR_DDRAM_LINE2					0x40

#define WSTAR_BIAS_OSC_FREQ_MASK			0x07
#define WSTAR_BIAS_OSC_BIAS_1_4				0x08
#define WSTAR_BIAS_OSC_CMD					0x10

#define WSTAR_ICON_RAM_ADD_MASK				0x0F
#define WSTAR_ICON_RAM_ADD_CMD				0x40

#define WSTAR_POW_ICON_CONTRAST_CNTH_MASK	0x03
#define WSTAR_POW_ICON_CONTRAST_BOOST		0x04
#define WSTAR_POW_ICON_CONTRAST_ICON		0x08
#define WSTAR_POW_ICON_CONTRAST_CMD			0x50

#define WSTAR_FOLLOWER_RATIO_MASK			0x07
#define WSTAR_FOLLOWER_ON					0x08
#define WSTAR_FOLLOWER_CMD					0x60

#define WSTAR_CONTRAST_LOW_MASK				0x0F
#define WSTAR_CONTRAST_LOW_CMD				0x70

#define CONTRAST_DEFAULT                0x04
#define FOLLOWER_DEFAULT                0x07
#define CONTRAST_MAX					0x3f

class Winstar : public Display {

 private:
  static const unsigned int freqency_3_volts[8];
  static const unsigned int freqency_5_volts[8];

  unsigned char entry_mode;
  unsigned char display_mode;
  unsigned char cursor_display_shift;
  unsigned char function_set;
  unsigned char bias_osc_frequency_adj;
  unsigned char icon_ram_address;
  unsigned char pow_icon_contrast;
  unsigned char follower;
  unsigned char contrast_set;
  unsigned char ddram_addr;
  int write_cmd(unsigned char data);
  int write_data(unsigned char data);
  int dpy_write(int type, uint8_t data);
  int set_normal_mode();
  int set_extended_mode();
  void init();

 public:
  Winstar(KeyPanel &key_panel, Scheduler &scheduler, Board& board);
  virtual ~Winstar();

  int device_init() {
    int ret;
    this->display_mode = WSTAR_DISPLAY_STATE | WSTAR_DISPLAY_CMD;
    this->function_set = WSTAR_FUNCTION_8BIT | WSTAR_FUNCTION_TWO_LINE_5X8
        | WSTAR_FUNCTION_CMD;
    this->entry_mode = WSTAR_ENTRY_MODE_CMD;
    this->cursor_display_shift = WSTAR_CUR_DPY_SHIFT_RIGHT
        | WSTAR_CUR_DPY_SHIFT_SCREEN | WSTAR_CUR_DPY_SHIFT_CMD;
    this->ddram_addr = WSTAR_DDRAM_CMD;
    // Extended instruction set (IS=1)
    this->bias_osc_frequency_adj = 0x04 | WSTAR_BIAS_OSC_CMD;
    this->icon_ram_address = WSTAR_ICON_RAM_ADD_CMD;
    this->pow_icon_contrast = WSTAR_POW_ICON_CONTRAST_BOOST
        | WSTAR_POW_ICON_CONTRAST_CMD;
    this->follower = FOLLOWER_DEFAULT | WSTAR_FOLLOWER_ON | WSTAR_FOLLOWER_CMD;
    this->contrast_set = CONTRAST_DEFAULT | WSTAR_CONTRAST_LOW_CMD;

    ret = write_cmd(function_set);
    ret = set_extended_mode();
    ret = write_cmd(bias_osc_frequency_adj);
    ret = write_cmd(contrast_set);
    ret = write_cmd(pow_icon_contrast);
    ret = write_cmd(follower);
    ret = write_cmd(display_mode);
    return (ret);
  }
  int clear() {
    return (write_cmd(WSTAR_CLEAR_DISPLAY_CMD));
  }
  int home() {
    return (write_cmd(WSTAR_RETURN_HOME_CMD));
  }
  int set_state(bool state) {
    int ret = -1;
    unsigned char oldreg = display_mode;
    (state ? display_mode |= WSTAR_DISPLAY_STATE : display_mode &=
                 ~WSTAR_DISPLAY_STATE);
    ret = write_cmd(display_mode);
    if (ret < 0)
      display_mode = oldreg;
    return (ret);
  }
  bool is_display_on() {
    return ((display_mode & WSTAR_DISPLAY_STATE) != 0);
  }
  int set_cursor_on(bool state) {
    (state ? display_mode |= WSTAR_DISPLAY_CURSOR : display_mode &=
                 ~WSTAR_DISPLAY_CURSOR);
    return (write_cmd(display_mode));
  }
  int set_cursor_blink(bool state) {
    (state ? display_mode |= WSTAR_DISPLAY_BLINK : display_mode &=
                 ~WSTAR_DISPLAY_BLINK);
    return (write_cmd(display_mode));
  }
  bool is_cursor_on() {
    return ((display_mode & WSTAR_DISPLAY_CURSOR) != 0);
  }
  int get_contrast() {
    return ((int) (((pow_icon_contrast & WSTAR_POW_ICON_CONTRAST_CNTH_MASK) << 4)
        | (contrast_set & WSTAR_CONTRAST_LOW_MASK)));
  }
  int set_contrast(int value) {
    int ret = -2;
    unsigned char oldreg;
    set_extended_mode();
    if (value > CONTRAST_MAX || value < 0)
      return (-2);
    oldreg = contrast_set;
    contrast_set = (value & WSTAR_CONTRAST_LOW_MASK);
    ret = write_cmd(contrast_set);
    if (ret < 0) {
      contrast_set = oldreg;
      return (ret);
    }
    oldreg = pow_icon_contrast;
    pow_icon_contrast |= ((value >> 4) & WSTAR_POW_ICON_CONTRAST_CNTH_MASK);
    ret = write_cmd(pow_icon_contrast);
    if (ret < 0) {
      pow_icon_contrast = oldreg;
      return (ret);
    }
    return (ret);
  }
  int set_insert_mode() {
    entry_mode |= WSTAR_ENTRY_MODE_SHIFT_ON;
    return (write_cmd(entry_mode));
  }
  int set_overwrite_mode() {
    entry_mode &= ~WSTAR_ENTRY_MODE_SHIFT_ON;
    return (write_cmd(entry_mode));
  }
  int shift_line() {
    set_normal_mode();
    cursor_display_shift |= WSTAR_CUR_DPY_SHIFT_SCREEN;
    return (write_cmd(cursor_display_shift));
  }
  int shift_cursor() {
    set_normal_mode();
    cursor_display_shift &= ~WSTAR_CUR_DPY_SHIFT_SCREEN;
    return (write_cmd(cursor_display_shift));
  }
  int set_double_height() {
    function_set &= ~WSTAR_FUNCTION_ONE_LINE_5X8_MASK;
    function_set |= WSTAR_FUNCTION_ONE_LINE_5X16;
    return (write_cmd(function_set));
  }
  int set_two_lines() {
    function_set &= ~WSTAR_FUNCTION_ONE_LINE_5X8_MASK;
    function_set &= ~WSTAR_FUNCTION_ONE_LINE_5X16;
    function_set |= WSTAR_FUNCTION_TWO_LINE_5X8;
    return (write_cmd(function_set));
  }
  int set_one_line() {
    function_set &= ~WSTAR_FUNCTION_ONE_LINE_5X8_MASK;
    function_set &= ~WSTAR_FUNCTION_ONE_LINE_5X16;
    return (write_cmd(function_set));
  }
  bool is_two_lines() {
    return ((function_set & WSTAR_FUNCTION_TWO_LINE_5X8) != 0);
  }
  int line2_home() {
    ddram_addr = WSTAR_DDRAM_CMD | WSTAR_DDRAM_LINE2;
    return (write_cmd(ddram_addr));
  }
  int set_backlight(bool state) {
    return (backlight_pin->setState((state ? STATE_ON : STATE_OFF)));
  }
  int reset() {
    reset_pin->flip(1000);  // 1 milli
    usleep(100000);         // 100 milli
    return (device_init());
  }
};

}

#endif /* WINSTAR_H_ */
