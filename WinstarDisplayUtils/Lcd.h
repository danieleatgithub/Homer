/*
 * Lcd.h
 *
 *  Created on: 19/apr/2015
 *      Author: daniele
 */

#ifndef LCD_H_
#define LCD_H_
#include <string>
#include "Pin.h"

namespace std {






class Lcd {
private:
	int fd;
	unsigned int address;
	string bus;
	string rst;
	string backlight;
	Pin*   backlight_pin;
	Pin*   reset_pin;

	static const unsigned int freqency_3_volts[8];
	static const unsigned int freqency_5_volts[8];

	unsigned char reg_display; 			// Control display register
	unsigned char reg_bias_frequency; 	// Bias Frequency
	unsigned char reg_function_set; // Function set register
	unsigned char reg_contrast_set; // Contrast set register
	unsigned char reg_power_icon; 	// Power icon and contrast high bits
	unsigned char reg_follower; 	// Follower register
	bool		  backlight_state;

	int write_cmd(unsigned char data);
	int write_data(unsigned char data);
	int lcd_write(int type,unsigned char data);
public:

	Lcd(const char *bus, const char *rst, const char *backlight);
	virtual ~Lcd();
	int 			lcd_open();
	int 			lcd_close();
	int 			setStatus(State_e state);
	State_e 	getStatus();
	bool 			isCursorON();
	unsigned int	getContrast();
	int				setContrast(unsigned int value);
	int 			setCursor(bool state,bool blink);
	int				lcd_putchar(unsigned char ch);
	int 			lcd_puts(char *str);
	int 			setBacklight(State_e state);
	State_e			getBacklight();
	int 			lcd_reset();
	int 			clear();
	int 			home();
};

} /* namespace std */

#endif /* LCD_H_ */
