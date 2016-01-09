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
class Lcd {

private:
	int fd;
	uint8_t address;
	string bus;
	string rst;
	string backlight;
	Pin* backlight_pin;
	Pin* reset_pin;

	static const unsigned int freqency_3_volts[8];
	static const unsigned int freqency_5_volts[8];

	uint8_t reg_display;                    // Control display register
	uint8_t reg_bias_frequency;     // Bias Frequency
	uint8_t reg_function_set; // Function set register
	uint8_t reg_contrast_set; // Contrast set register
	uint8_t reg_power_icon;         // Power icon and contrast high bits
	uint8_t reg_follower;   // Follower register
	bool backlight_state;
	union entry_mode_set_u entry_mode;

	int write_cmd(uint8_t data);
	int write_data(uint8_t data);
	int lcd_write(int type, uint8_t data);
public:

	Lcd(const char *bus, const char *rst, const char *backlight);
	virtual ~Lcd();
	int lcd_open();
	int lcd_close();
	int setStatus(State_e state);
	State_e getStatus();
	bool isCursorON();
	unsigned int getContrast();
	int setContrast(uint8_t value);
	int setCursor(bool state, bool blink);
	int lcd_putchar(unsigned char ch);
	int lcd_puts(char *str);
	int setBacklight(State_e state);
	State_e getBacklight();
	int lcd_reset();
	int clear();
	int clear(bool home);
	int home();
	int set_direction(dpy::Direction_e dir);
	int set_shift(State_e state);

};
}
#endif
