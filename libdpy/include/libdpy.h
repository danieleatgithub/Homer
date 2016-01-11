#ifndef libdpy_H
#define libdpy_H

#include "stdio.h"

#include <string>
#include <stdint.h>

#include "winstar.h"
#include "pin.h"

using namespace std;

namespace dpy {

enum Direction_e {
	LEFT = 0,
	RIGHT
};
class Dpy {

private:
	int fd;
	uint8_t address;
	string bus;
	string rst;
	int write_usleep;

	string backlight;
	Pin* backlight_pin;
	Pin* reset_pin;

	static const unsigned int freqency_3_volts[8];
	static const unsigned int freqency_5_volts[8];

	bool backlight_state;

	union entry_mode_set_u entry_mode;
	union display_mode_set_u display_mode;
	union cursor_display_shift_u cursor_display_shift;
	union function_set_u function_set;
	union bias_osc_frequency_adj_u bias_osc_frequency_adj;
	union icon_ram_address_u icon_ram_address;
	union pow_icon_contrast_u pow_icon_contrast;
	union follower_u follower;
	union contrast_set_u contrast_set;


	int write_is0_cmd(unsigned char data);
	int write_is1_cmd(unsigned char data);
	int write_data(unsigned char data);
	int dpy_write(int type, unsigned char data);
	int init();
public:

	Dpy(const char *bus, const char *rst, const char *backlight);
	virtual ~Dpy();
	int dpy_open();
	int dpy_close();
	int putchar(unsigned char ch);
	int puts(char *str);
	int reset();

	int set_state(State_e state);
	bool is_display_on();
	bool is_cursor_on();
	unsigned int get_contrast();
	int set_contrast(unsigned char value);
	int set_cursor(bool state, bool blink);
	int set_backlight(State_e state);
	bool is_backlight_on();
	int set_two_lines(State_e state);
	bool is_two_lines();
	int clear();
	int home();
	int set_direction(dpy::Direction_e dir);
	int set_shift(State_e state);
	int set_double_height(State_e state);

};
}
#endif
