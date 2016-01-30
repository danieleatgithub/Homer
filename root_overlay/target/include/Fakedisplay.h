/*
 * winstar.h
 *
 *  Created on: 06/gen/2016
 *      Author: daniele
 */

#ifndef FAKEDISPLAY_H_
#define FAKEDISPLAY_H_

#include <stdint.h>

#include "Winstar.h"

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

};

}

#endif /* WINSTAR_H_ */
