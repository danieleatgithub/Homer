/*
 * winstar.h
 *
 *  Created on: 06/gen/2016
 *      Author: daniele
 */

#ifndef WINSTAR_H_
#define WINSTAR_H_

#include "Display.h"

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

class Winstar: public Display {

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

    void init();

  public:
    Winstar(const char *bus);
    Winstar(const char *bus, const char *rst, const char *backlight);
    virtual ~Winstar();

    int set_state(bool state);
    bool is_display_on();
    bool is_cursor_on();
    int get_contrast();
    int set_contrast(int value);
    int set_cursor_on(bool state);
    int set_cursor_blink(bool state);
    int set_backlight(bool state);
    bool is_backlight_on();
    int set_two_lines();
    int set_one_line();
    int set_double_height();
    bool is_two_lines();
    int clear();
    int home();
    int line2_home();

    int shift_line();
    int shift_cursor();
    int set_insert_mode();
    int set_overwrite_mode();

    int set_extended_mode();
    int set_normal_mode();
    int write_cmd(unsigned char data);
    virtual int dpy_write(int type, unsigned char data);
    virtual int device_init();
    virtual int write_data(unsigned char data);
};

}

#endif /* WINSTAR_H_ */
