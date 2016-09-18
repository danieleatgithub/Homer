/*
 * emulator.hpp
 *
 *  Created on: 22/ago/2016
 *      Author: daniele
 */

#ifndef HOMEREMULATOR_HPP_
#define HOMEREMULATOR_HPP_
#include <GL/glut.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <KeyEmulator.hpp>
#include <HwEmulated.hpp>
#include <Scheduler.hpp>
#include <WinstarEmulator.hpp>

#define HEMUL_REFRESHRATE 250 // milliseconds

using namespace homerio;

namespace homeremulator {
class HomerEmulator;

struct GL_callbacks {
	static void  reshape(int w, int h);
	static void  keypress(unsigned char key, int x, int y);
	static void  keyrelease(unsigned char key, int x, int y);
	static void  display();
	static void  timer(int value);
	static void  idle();


	void setHomerEmulator(HomerEmulator* e) {
		GL_callbacks::homerEmulator = e;
	}
	void setDisplayEmulator(WinstarEmulator* d) {
		GL_callbacks::displayEmulator = d;
	}

	static HomerEmulator 	*homerEmulator;
	static WinstarEmulator	*displayEmulator;


};


class HomerEmulator {
private:
	char *myargv [1];
	int myargc;

	unsigned int		refreshRate;
public:
	KeyEmulator keyEmulator;
	GL_callbacks callbacks;

	void makeRasterFont(void);
	const string &getKeyEventFilename() {
		return keyEmulator.getEvent();
	}

public:

	HomerEmulator(WinstarEmulator *emulatedDisplay);
	virtual ~HomerEmulator() {};
	int start();
	int stop();
	void mainLoop();

	unsigned int getRefreshRate() const {
		return refreshRate;
	}
	void setRefreshRate(unsigned int refreshRate) {
		this->refreshRate = refreshRate;
	}
};

}


#endif /* HOMEREMULATOR_HPP_ */
