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

	unsigned char entry_mode;
	unsigned char display_mode;
	unsigned char cursor_display_shift;
	unsigned char function_set;
	unsigned char bias_osc_frequency_adj;
	unsigned char icon_ram_address;
	unsigned char pow_icon_contrast;
	unsigned char follower;
	unsigned char contrast_set;


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

	int set_state(bool state);
	bool is_display_on();
	bool is_cursor_on();
	int get_contrast();
	int set_contrast(int value);
	int set_cursor(bool state, bool blink);
	int set_backlight(bool state);
	bool is_backlight_on();
	int set_two_lines(bool state);
	bool is_two_lines();
	int clear();
	int home();
	int set_shift(bool enabled, bool screen);
	int set_double_height(bool state);

};
}
#endif
