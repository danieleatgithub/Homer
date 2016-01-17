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
	this->backlight_state = true;


	this->entry_mode = 0;
	this->display_mode = 0;
	this->function_set = 0;
	this->ddram_addr = 0;
	// Normal instruction set (IS=0)
	this->cursor_display_shift = 0;
	// Extended instruction set (IS=1)
	this->bias_osc_frequency_adj = 0;
	this->icon_ram_address = 0;
	this->pow_icon_contrast = 0;
	this->follower = 0;
	this->contrast_set = 0;

	pin = Pin::getPinDescriptor(this->rst.c_str());
	reset_pin = new Pin(pin);
	reset_pin->pin_export();
	reset_pin->set_direction(OUT);
	reset_pin->pin_open();
	reset_pin->setState(STATE_ON);

	pin = Pin::getPinDescriptor(this->backlight.c_str());
	backlight_pin = new Pin(pin);
	backlight_pin->pin_export();
	backlight_pin->set_direction(OUT);
	backlight_pin->pin_open();

}

Dpy::~Dpy() {
	dpy_close();
	reset_pin->pin_close();
	backlight_pin->pin_close();
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

	init();
	return (ret);
}

int Dpy::dpy_close() {
	int ret;
	ret = close(this->fd);
	return (ret);
}

int Dpy::reset() {
	reset_pin->flip(1000); // 1 milli
	usleep(100000); // 100 milli
	init();
}
int Dpy::set_backlight(bool state) {
	return (backlight_pin->setState((state ? STATE_ON : STATE_OFF)));
}
int Dpy::clear() {
	return (write_cmd(WSTAR_CLEAR_DISPLAY_CMD));
}
int Dpy::home() {
	return (write_cmd(WSTAR_RETURN_HOME_CMD));
}
int Dpy::set_state(bool state) {
	int ret = -1;
	unsigned char oldreg = display_mode;
	(state ? display_mode |= WSTAR_DISPLAY_STATE : display_mode &= ~WSTAR_DISPLAY_STATE);
	ret = write_cmd(display_mode);
	if (ret < 0) display_mode = oldreg;
	return (ret);
}
bool Dpy::is_display_on() {
	return((display_mode  & WSTAR_DISPLAY_STATE) != 0);
}
int Dpy::set_cursor_on(bool state) {
	(state ? display_mode |= WSTAR_DISPLAY_CURSOR : display_mode &= ~WSTAR_DISPLAY_CURSOR);
	return(write_cmd(display_mode));
}
int Dpy::set_cursor_blink(bool state) {
	(state ? display_mode |= WSTAR_DISPLAY_BLINK : display_mode &= ~WSTAR_DISPLAY_BLINK);
	return(write_cmd(display_mode));
}
bool Dpy::is_cursor_on() {
	return ((display_mode & WSTAR_DISPLAY_CURSOR ) != 0);
}
int Dpy::get_contrast() {
	return ((int)(	((pow_icon_contrast & WSTAR_POW_ICON_CONTRAST_CNTH_MASK) << 4) | (contrast_set & WSTAR_CONTRAST_LOW_MASK)));
}
int Dpy::set_contrast(int value) {
	int ret = -2;
	unsigned char oldreg;
	set_extended_mode();
	if (value > CONTRAST_MAX || value < 0 ) return (-2);
	oldreg = contrast_set;
	contrast_set = (value & WSTAR_CONTRAST_LOW_MASK);
	ret = write_cmd(contrast_set);
	if(ret < 0) {
		contrast_set = oldreg;
		return(ret);
	}
	oldreg = pow_icon_contrast;
	pow_icon_contrast |= ((value >> 4) & WSTAR_POW_ICON_CONTRAST_CNTH_MASK);
	ret = write_cmd(pow_icon_contrast);
	if(ret < 0) {
		pow_icon_contrast = oldreg;
		return(ret);
	}
	return (ret);
}
int Dpy::putchar(unsigned char ch) {
	return (write_data(ch));
}
int Dpy::puts(char *str) {
	char *p = str;
	unsigned int ret = 0;
	while (ret == 0 && p && *p)
		ret = write_data(*p++);
	return ((p ? (p - str) : -1));
}
bool Dpy::is_backlight_on() {
	return(backlight_state);
}
int Dpy::set_insert_mode() {
	entry_mode |= WSTAR_ENTRY_MODE_SHIFT_ON;
	return(write_cmd(entry_mode));
}
int Dpy::set_overwrite_mode() {
	entry_mode &= ~WSTAR_ENTRY_MODE_SHIFT_ON;
	return(write_cmd(entry_mode));
}
int Dpy::shift_line() {
	set_normal_mode();
	cursor_display_shift |= WSTAR_CUR_DPY_SHIFT_SCREEN;
	return(write_cmd(cursor_display_shift));
}
int Dpy::shift_cursor() {
	set_normal_mode();
	cursor_display_shift &= ~WSTAR_CUR_DPY_SHIFT_SCREEN;
	return(write_cmd(cursor_display_shift));
}
int Dpy::set_double_height() {
	function_set &= ~WSTAR_FUNCTION_ONE_LINE_5X8_MASK;
	function_set |= WSTAR_FUNCTION_ONE_LINE_5X16;
	return(write_cmd(function_set));
}
int Dpy::set_two_lines() {
	function_set &= ~WSTAR_FUNCTION_ONE_LINE_5X8_MASK;
	function_set &= ~WSTAR_FUNCTION_ONE_LINE_5X16;
	function_set |= WSTAR_FUNCTION_TWO_LINE_5X8;
	return(write_cmd(function_set));
}
int Dpy::set_one_line() {
	function_set &= ~WSTAR_FUNCTION_ONE_LINE_5X8_MASK;
	function_set &= ~WSTAR_FUNCTION_ONE_LINE_5X16;
	return(write_cmd(function_set));
}
bool Dpy::is_two_lines() {
	return((function_set  & WSTAR_FUNCTION_TWO_LINE_5X8) != 0);
}
int Dpy::line2_home() {
	ddram_addr = WSTAR_DDRAM_CMD | WSTAR_DDRAM_LINE2;
	return(write_cmd(ddram_addr));
}
// private methods

int Dpy::init() {
	int ret;
	this->backlight_state = true;
	this->display_mode = WSTAR_DISPLAY_STATE | WSTAR_DISPLAY_CMD;
	this->function_set = WSTAR_FUNCTION_8BIT | WSTAR_FUNCTION_TWO_LINE_5X8 | WSTAR_FUNCTION_CMD;
	this->entry_mode = WSTAR_ENTRY_MODE_CMD;
	this->cursor_display_shift = WSTAR_CUR_DPY_SHIFT_RIGHT | WSTAR_CUR_DPY_SHIFT_SCREEN | WSTAR_CUR_DPY_SHIFT_CMD;
	this->ddram_addr = WSTAR_DDRAM_CMD;
	// Extended instruction set (IS=1)
	this->bias_osc_frequency_adj = 0x04 | WSTAR_BIAS_OSC_CMD;
	this->icon_ram_address = WSTAR_ICON_RAM_ADD_CMD;
	this->pow_icon_contrast = WSTAR_POW_ICON_CONTRAST_BOOST | WSTAR_POW_ICON_CONTRAST_CMD;
	this->follower = FOLLOWER_DEFAULT | WSTAR_FOLLOWER_ON | WSTAR_FOLLOWER_CMD;
	this->contrast_set = CONTRAST_DEFAULT | WSTAR_CONTRAST_LOW_CMD;
	set_backlight(backlight_state);
	ret = write_cmd(function_set);
	ret = set_extended_mode();
	ret = write_cmd(bias_osc_frequency_adj);
	ret = write_cmd(contrast_set);
	ret = write_cmd(pow_icon_contrast);
	ret = write_cmd(follower);
	ret = write_cmd(display_mode);
	return(ret);
}
int Dpy::set_normal_mode() {
	function_set &=  ~WSTAR_FUNCTION_EXTENDED;
	return(dpy_write(0,function_set));
}
int Dpy::set_extended_mode() {
	function_set |= WSTAR_FUNCTION_EXTENDED;
	return(dpy_write(0,function_set));
}
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

