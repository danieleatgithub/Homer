//============================================================================
// Name        : hemu.cpp
// Author      : Daniele Colombo
// Version     :
// Copyright   : GPL 2
// Description : Hello World in C++, Ansi-style
//============================================================================
#include <GL/freeglut.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <HomerEmulator.hpp>
#include <memory>
#include <HwEmulated.hpp>

using namespace std;
using namespace homerio;

namespace homeremulator {


GLuint fontOffset;

GLfloat white[3] = { 1.0, 1.0, 1.0 };
GLfloat green[3] = { 0.0, 1.0, 0.0 };
GLfloat black[3] = { 0.0, 0.0, 0.0 };
GLfloat grey[3] = { 0.5, 0.5, 0.5 };
GLfloat lightgrey[3] = { 0.9, 0.9, 0.9 };

HomerEmulator* GL_callbacks::homerEmulator = nullptr;
WinstarEmulator* GL_callbacks::displayEmulator = nullptr;

	void GL_callbacks::reshape(int w, int h)
	{
	   glViewport(0, 0, (GLsizei) w, (GLsizei) h);
	   glMatrixMode(GL_PROJECTION);
	   glLoadIdentity();
	   glOrtho (0.0, w, 0.0, h, -1.0, 1.0);
	   glMatrixMode(GL_MODELVIEW);
	}

	void GL_callbacks::keypress(unsigned char key, int x, int y) {
		homerEmulator->keyEmulator.gl_key_press(key,x,y);
	}
	void GL_callbacks::keyrelease(unsigned char key, int x, int y) {
		homerEmulator->keyEmulator.gl_key_release(key,x,y);
	}

	void GL_callbacks::display(void)
	{
	   glClear(GL_COLOR_BUFFER_BIT);
	   displayEmulator->draw();
	   glFlush();
	}


	void GL_callbacks::timer(int value)
	{
		glutPostRedisplay();
		glutTimerFunc(homerEmulator->getRefreshRate(), GL_callbacks::timer, value);
	}


	HomerEmulator::HomerEmulator(WinstarEmulator *emulatedDisplay) {
		myargc = 1;
		myargv [0]=strdup ("Homer Emulator");
		this->refreshRate = HEMUL_REFRESHRATE;
		callbacks.setHomerEmulator(this);
		callbacks.setDisplayEmulator(emulatedDisplay);
	}

	int HomerEmulator::start() {
		   keyEmulator.gl_start();
		   glutInit(&myargc, &myargv[0]);
		   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
		   glutInitWindowSize(400, 200);
		   glutInitWindowPosition (100, 100);
		   glutCreateWindow("Homer");
		   glShadeModel (GL_FLAT);
		   glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE,GLUT_ACTION_GLUTMAINLOOP_RETURNS);
		   glutReshapeFunc(GL_callbacks::reshape);
		   glutSetKeyRepeat	(GLUT_KEY_REPEAT_OFF );
		   glutKeyboardFunc(GL_callbacks::keypress);
		   glutKeyboardUpFunc(GL_callbacks::keyrelease);
		   glutDisplayFunc(GL_callbacks::display);
		   glutTimerFunc(0, GL_callbacks::timer, 0);
		   return 0;
	}

	int HomerEmulator::stop() {
	    keyEmulator.gl_stop();
		return 0;
	}

	void HomerEmulator::mainLoop() {
		glutMainLoop();
	}

}


