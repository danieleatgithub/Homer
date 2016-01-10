#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <string>
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/configurator.h>
#include <iomanip>
#include "winstar.h"
#include "pin.h"
#include "libdpy.h"

namespace dpy {

const unsigned int freqency_3_volts[8] = { 122, 131, 144, 161, 183, 221, 274, 347 };
const unsigned int freqency_5_volts[8] = { 120, 133, 149, 167, 192, 227, 277, 347 };

Dpy::Dpy(const char *bus, const char *rst, const char *backlight) {
	struct pinmap_s *pin;

	this->write_usleep = 100;
	this->fd = -1;
	this->bus = string(bus);
	this->rst = string(rst);
	this->backlight = string(backlight);
	this->address = WINSTAR_I2C_ADD;
	this->backlight_state = false;

	this->display_mode.reg 				= { 0b0, 0b0, 0b1, 0b00001 };
	this->function_set.reg  			= { 0b0, 0b0, 0b0, 0b1, 0b1, 0b001 };
	this->bias_osc_frequency_adj.reg 	= { 0b100, 0b0, 0b0001 };
	this->contrast_set.reg 				= { CONTRAST_DEFAULT, 0b0111 };
	this->pow_icon_contrast.reg 		= { 0b00, 0b1, 0b0, 0b0101 };
	this->follower.reg 					= { FOLLOWER_DEFAULT, 0b1, 0b0110 };
	this->entry_mode.reg 				= { 0b0, 0b1,  0b000001};
	this->cursor_display_shift.reg  	= { 0b00, 0b1, 0b1, 0b0001 };
	this->icon_ram_address.reg 			= { 0b0, 0b0100 };

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

Dpy::~Dpy() {
	dpy_close();
	delete (reset_pin);
	delete (backlight_pin);
}
int Dpy::dpy_open() {
	int ret = 0;

	// Open i2c
	fd = open(bus.c_str(), O_RDWR);
	if (fd < 0)
		return (fd);
	if (ioctl(fd, I2C_SLAVE, this->address) < 0) {
		printf("ioctl error: %s\n", strerror(errno));
		close(fd);
		return -1;
	}

	ret = write_cmd(function_set.raw);
	function_set.reg.extended_instruction_set = 1;
	ret = write_cmd(function_set.raw);
	ret = write_cmd(bias_osc_frequency_adj.raw);
	ret = write_cmd(contrast_set.raw);
	ret = write_cmd(pow_icon_contrast.raw);
	ret = write_cmd(follower.raw);
	ret = write_cmd(display_mode.raw);

	reset_pin->pin_open();
	backlight_pin->pin_open();

	return (ret);
}

int Dpy::dpy_close() {
	int ret;
	ret = close(this->fd);
	ret = reset_pin->pin_close();
	ret = backlight_pin->pin_close();
	return (ret);
}

int Dpy::dpy_reset() {
	return (reset_pin->flip(1000));
}
int Dpy::set_backlight(State_e state) {
	return (backlight_pin->setState(state));
}
int Dpy::clear() {
	return (write_cmd(CLEAR_DISPLAY_CMD));
}

int Dpy::clear(bool home) {
	int ret;
	ret = write_cmd(CLEAR_DISPLAY_CMD);
	if (home) ret += write_cmd(RETURN_HOME_CMD);
	return(ret);
}

int Dpy::home() {
	return (write_cmd(RETURN_HOME_CMD));
}

int Dpy::set_state(State_e state) {
	int ret = -1;
	unsigned char oldreg = display_mode.raw;
	if (state == STATE_ON) display_mode.reg.display_on = 1;
	if (state == STATE_OFF) display_mode.reg.display_on = 0;
	if (state == STATE_TOGGLE) display_mode.reg.display_on = !display_mode.reg.display_on;
	ret = write_cmd(display_mode.raw);
	if (ret < 0) display_mode.raw = oldreg;
	return (ret);
}
bool Dpy::is_display_on() {
	return(display_mode.reg.display_on == 1);
}
int Dpy::set_cursor(bool state, bool blink) {
	int ret = -1;
	unsigned char oldreg = display_mode.raw;
	if(state) display_mode.reg.cursor_on = 1;
	else      display_mode.reg.cursor_on = 0;
	if(blink) display_mode.reg.blink_on  = 1;
	else      display_mode.reg.blink_on  = 0;
	ret = write_cmd(display_mode.raw);
	if (ret < 0) display_mode.raw = oldreg;
	return (ret);
}

bool Dpy::is_cursor_on() {
	return (display_mode.reg.cursor_on == 1);
}
unsigned int Dpy::get_contrast() {
	return ((unsigned int)(	pow_icon_contrast.reg.contrast_high << 4 |
							contrast_set.reg.contrast_low));
}
int Dpy::set_contrast(uint8_t value) {
	int ret = -2;
	unsigned char oldreg;
	if (value > CONTRAST_MAX) return (-2);
	oldreg = pow_icon_contrast.raw;
	pow_icon_contrast.reg.contrast_high = (value >> 4);
	ret = write_cmd(pow_icon_contrast.raw);
	if(ret < 0) {
		pow_icon_contrast.raw = oldreg;
		return(ret);
	}
	oldreg = contrast_set.raw;
	contrast_set.reg.contrast_low = (value & 0xf0);
	ret = write_cmd(contrast_set.raw);
	if(ret < 0) {
		contrast_set.raw = oldreg;
		return(ret);
	}
	return (ret);
}
int Dpy::dpy_putchar(unsigned char ch) {
	return (write_data(ch));
}
int Dpy::dpy_puts(char *str) {
	char *p = str;
	unsigned int ret = 0;
	while (ret == 0 && p && *p)
		ret = write_data(*p++);
	return ((p ? (p - str) : -1));
}

int Dpy::set_direction(dpy::Direction_e dir) {
	entry_mode.reg.versus = dir;
	return(write_cmd(entry_mode.raw));
}

int Dpy::set_shift(State_e state) {
	if(state == STATE_TOGGLE) entry_mode.reg.shift = !entry_mode.reg.shift;
	if(state == STATE_ON) entry_mode.reg.shift = 1;
	if(state == STATE_OFF) entry_mode.reg.shift = 0;
	return(write_cmd((uint8_t)entry_mode.raw));
}

// private methods

int Dpy::write_cmd(unsigned char data) {
	return (this->dpy_write(0, data));
}
int Dpy::write_data(uint8_t data) {
	return (this->dpy_write(0x40, data));
}
int Dpy::dpy_write(int type, uint8_t data) {
	unsigned char buffer[2];
	buffer[0] = (unsigned char) type;
	buffer[1] = data;
	printf("DEBUG i2cset -y 0x%x 0x%x 0x%x\n", address, type, data);
	if (write(fd, buffer, 2) != 2) {
		printf("Error writing file: %s\n", strerror(errno));
		return -1;
	}
	fsync(fd);
	usleep(write_usleep);
	return (0);
}



}

