/*
 * WinstarEmulation.hpp
 *
 *  Created on: 24/ago/2016
 *      Author: daniele
 */

#ifndef WINSTAREMULATOR_HPP_
#define WINSTAREMULATOR_HPP_

#include <Winstar.h>
#include <EmuGlobals.h>
#include <Observer.h>
#include <GL/freeglut.h>

using namespace homeremulator;

namespace homerio {

class WinstarEmulator: public Winstar {
	GLfloat *bg,*fg;
	BoardEmulated& 		board;
	Registration		light_reg,i2cwr_reg;
	bool ready;
	char line1[18];
	char line2[18];
	char *cursor;
	void printString(char *s)
	{
	   glutBitmapString(GLUT_BITMAP_9_BY_15, (const unsigned char *)s);

	}
public:

	WinstarEmulator(KeyPanel &kpnl, Scheduler &shd, BoardEmulated& board) :
		Winstar(kpnl,shd, board), board(board) {
		bg = grey;
		fg = black;
		cursor = &line1[0];
		ready = false;
		backLight_register();
		i2cbus_register();
		sprintf(line1,"Homer Emulator");
		sprintf(line2,"1234567890123456");
	}

	virtual ~WinstarEmulator() {};

	GpioPort& getLcdReset() {
		return(board.getLcdReset());
	}
	GpioPort& getLcdBacklight() {
		return(board.getLcdBacklight());
	}

	void i2cbus_register() {
		board.getEmulatedI2c0().reg_write(i2cwr_reg, WINSTAR_I2C_ADD, [&] (int fd, const void *buffer, size_t size) {
			const unsigned char *p = (const unsigned char *)buffer;
			// FIXME: change GL init dependency logic
			if(!ready) return;
			if(size == 2) {
				if(p[0] == WSTAR_DATA) {
					*cursor++ = p[1];
					this->draw();
				}
				if(p[0] == WSTAR_CMD && p[1] == WSTAR_CLEAR_DISPLAY_CMD) {
					sprintf(line1,"                ");
					sprintf(line2,"                ");
					cursor = &line1[0];
					this->draw();
				}
				if(p[0] == WSTAR_CMD && p[1] == (WSTAR_DDRAM_CMD | WSTAR_DDRAM_LINE2)) {
					cursor = &line2[0];
				}
			}
			cerr << "SZ=" << size << " p[0]=" << hex << p[0] << endl;
		});
	}
	void backLight_register() {
		board.getEmulatedLcdBacklight().reg_write(light_reg, [&] ( int fd, const void *buffer, size_t size ) {
		   	   const unsigned char *p = (const unsigned char *)buffer;
		   	   if(p[0] == 0x30) {
		   		   this->setColor(grey,black);
		   	   } else {
		   		this->setColor(white,black);
		   	   }
		   	   glutPostRedisplay();

		});
	}

	void draw() {
		   glColor3fv(bg);
		   glRects(10.0f,140.0f, 190.0f, 190.0f);
		   glColor3fv(fg);
		   glRasterPos2i(20, 170);
		   printString(line1);
		   glRasterPos2i(20, 150);
		   printString(line2);
	}

	void setColor(GLfloat *background,GLfloat *foreground) {
			   bg = background;
			   fg = foreground;
	}
    int reset() {
    	int ret = Winstar::reset();
		if(ret >= 0)ready = true;
		return(ret);
    }

};


} /* namespace homerio */

#endif /* WINSTAREMULATOR_HPP_ */
