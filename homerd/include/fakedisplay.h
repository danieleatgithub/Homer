/*
 * winstar.h
 *
 *  Created on: 06/gen/2016
 *      Author: daniele
 */

#ifndef FAKEDISPLAY_H_
#define FAKEDISPLAY_H_
#include <stdint.h>
#include "winstar.h"

namespace homerio {


class FakeDisplay: public Winstar {

private:


public:
	FakeDisplay(const char *bus);
	FakeDisplay(const char *bus, const char *rst, const char *backlight);
	virtual ~FakeDisplay();
	int dpy_open();
	int dpy_close();
	virtual int dpy_write(int type, uint8_t data);

	virtual int set_state(bool state) { return(0); };
	virtual bool is_display_on() { return(true); };
	virtual bool is_cursor_on() { return(true); };
	virtual int get_contrast() { return(0); };
	virtual int set_contrast(int value) { return(0); };
	virtual int set_cursor_on(bool state) { return(0); };
	virtual int set_cursor_blink(bool state) { return(0); };
	virtual int set_backlight(bool state) { return(0); };
	virtual bool is_backlight_on() { return(0); };
	virtual int set_two_lines() { return(0); };
	virtual int set_one_line() { return(0); };
	virtual int set_double_height() { return(0); };
	virtual bool is_two_lines() { return(true); };
	virtual int clear() { return(0); };
	virtual int home() { return(0); };
	virtual int line2_home() { return(0); };
	virtual int shift_line() { return(0); };
	virtual int shift_cursor() { return(0); };
	virtual int set_insert_mode() { return(0); };
	virtual int set_overwrite_mode() { return(0); };

};

}


#endif /* WINSTAR_H_ */
