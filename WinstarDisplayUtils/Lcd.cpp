/*
 * Lcd.cpp
 *
 *  Created on: 19/apr/2015
 *      Author: daniele
 */


#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <string>
#include "winstar.h"
#include "Pin.h"
#include "Lcd.h"


namespace std {

const unsigned int freqency_3_volts[8] = {122,131,144,161,183,221,274,347};
const unsigned int freqency_5_volts[8] = {120,133,149,167,192,227,277,347};

Lcd::Lcd(const char *bus, const char *rst, const char *backlight) {
	struct pinmap_s *pin;

	this->fd = -1;
	this->bus = string(bus);
	this->rst = string(rst);
	this->backlight = string(backlight);
	this->address = WINSTAR_I2C_ADD;
	this->reg_display 			= DISPLAY_CMD_CTRL | DISPLAY_ON;
	this->reg_function_set 		= FUNCTION_SET_CMD 	| FUNCTION_SET_DL | FUNCTION_SET_2LINES;
	this->reg_bias_frequency	= BIAS_FREQ_CMD 	| BIAS_FREQ_3V_DEFAULT;
	this->reg_contrast_set 		= CONTRAST_SET_CMD 	| CONTRAST_DEFAULT;
	this->reg_power_icon		= POWER_ICON_CMD 	| POWER_ICON_BOOST;
	this->reg_follower			= FOLLOWER_CMD 		| FOLLOWER_ON | FOLLOWER_DEFAULT;
	this->backlight_state 		= false;

	pin = Pin::getPinDescriptor(this->rst.c_str());
	reset_pin = new Pin(pin);
	reset_pin->pin_export();
	reset_pin->set_direction(OUT);
	usleep(1000);
	reset_pin->setState(STATE_ON); // Remove reset state after direction change

	pin = Pin::getPinDescriptor(this->backlight.c_str());
	backlight_pin = new Pin(pin);
	backlight_pin->pin_export();
	backlight_pin->set_direction(OUT);
	backlight_pin->setState(STATE_ON); // Set default Display state
}

Lcd::~Lcd() {
	lcd_close();
	delete(reset_pin);
	delete(backlight_pin);
}
int Lcd::lcd_open() {
	int ret=0;

	// Open i2c
	fd = open(bus.c_str(), O_RDWR);
	if(fd < 0) return(fd);
	if (ioctl(fd, I2C_SLAVE, this->address) < 0) {
			printf("ioctl error: %s\n", strerror(errno));
			close(fd);
			return -1;
	}

	ret = write_cmd(reg_function_set);
	reg_function_set |= FUNCTION_SET_EXTENS;
	ret = write_cmd(reg_function_set);
	ret = write_cmd(reg_bias_frequency);
	ret = write_cmd(reg_contrast_set);
	ret = write_cmd(reg_power_icon);
	ret = write_cmd(reg_follower);
	ret = write_cmd(reg_display);

	reset_pin->pin_open();
	backlight_pin->pin_open();

	return(ret);
}

int Lcd::lcd_close(){
	int ret;
	ret = close(this->fd);
	ret = reset_pin->pin_close();
	ret = backlight_pin->pin_close();
	return(ret);
}

int Lcd::lcd_reset() {
	return(reset_pin->flip(1000));
}
int Lcd::setBacklight(State_e state) {
	return(backlight_pin->setState(state));
}
int Lcd::clear(){
	return(write_cmd(CLEAR_DISPLAY_CMD));
}
int Lcd::home(){
	return(write_cmd(RETURN_HOME_CMD));
}

int Lcd::setStatus(State_e state) {
	int ret = -1;
	uint8_t newreg = reg_display;
	if(state == STATE_ON || (state == STATE_TOGGLE && !(reg_display & DISPLAY_ON) )) newreg = this->reg_display & DISPLAY_ON;
	if(state == STATE_OFF || (state == STATE_TOGGLE && (reg_display & DISPLAY_ON)))  newreg = this->reg_display & ~DISPLAY_ON;
	ret = write_cmd(newreg);
	if(ret>0) this->reg_display = newreg;
	return(ret);
}
State_e Lcd::getStatus() {
	if((reg_display & DISPLAY_ON)) return(STATE_ON);
	else return(STATE_OFF);
}
int Lcd::setCursor(bool state,bool blink) {
	int ret = -1;
	unsigned char newreg = reg_display;
	newreg = reg_display |
			(state ? DISPLAY_CURSOR : ~DISPLAY_CURSOR) |
			(blink ? DISPLAY_CURSOR_BLINK : ~DISPLAY_CURSOR_BLINK);
	ret = write_cmd(newreg);
	if(ret>0) this->reg_display = newreg;
	return(ret);
}

bool Lcd::isCursorON() {
	return((reg_display & DISPLAY_CURSOR) != 0);
}
unsigned int Lcd::getContrast() {
	return(((reg_power_icon & POWER_ICON_CONTR_MASK) << 4) && (reg_contrast_set & CONTRAST_MAX));
}
int Lcd::setContrast(uint8_t value) {
	int ret;
	unsigned char reg = reg_contrast_set;
	if(value > CONTRAST_MAX) return(-1);
	reg &= CONTRAST_MASK & value;
	ret = write_cmd(reg);
	if(ret < 0) return(ret);
	reg_contrast_set = reg;
	reg = reg_power_icon;
	if((reg & POWER_ICON_CONTR_MASK) != (value & POWER_ICON_CONTR_MASK)) {
		reg |= POWER_ICON_CONTR_MASK | value;
		ret = write_cmd(reg);
		if(ret < 0) return(ret);
		reg_contrast_set = reg;
	}
	return(ret);
}
int Lcd::lcd_putchar(unsigned char ch) {
	return(write_data(ch));
}
int Lcd::lcd_puts(char *str){
	char 		 *p = str;
	unsigned int ret = 0;
	while(ret == 0 && p && *p) 	ret = write_data(*p++);
	return((p?(p-str):-1));
}

// private methods

int Lcd::write_cmd(unsigned char data){
	return(this->lcd_write(0,data));
}
int Lcd::write_data(uint8_t data){
	return(this->lcd_write(0x40,data));
}
int Lcd::lcd_write(int type,uint8_t data) {
	unsigned char buffer[2];
	buffer[0] = (unsigned char)type;
	buffer[1] = data;
	printf("DEBUG i2cset -y %s 0x%x 0x%x 0x%x\n",bus.c_str(),address,type,data);
	if (write(fd,buffer,2)!=2) {
			printf("Error writing file: %s\n", strerror(errno));
			return -1;
	}
	fsync(fd);
	usleep(1000);
	return(0);
}
} /* namespace std */
