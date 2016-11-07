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
GLuint texture;

GLfloat white[3] = { 1.0, 1.0, 1.0 };
GLfloat green[3] = { 0.0, 1.0, 0.0 };
GLfloat black[3] = { 0.0, 0.0, 0.0 };
GLfloat grey[3] = { 0.5, 0.5, 0.5 };
GLfloat lightgrey[3] = { 0.9, 0.9, 0.9 };

HomerEmulator* GL_callbacks::homerEmulator = nullptr;
WinstarEmulator* GL_callbacks::displayEmulator = nullptr;

GLuint GL_callbacks::LoadBMPTexture(const char *filename, unsigned int width,
		unsigned int height) {
	GLuint texture;
	void *buf;
	FILE* fp = fopen(filename, "r");
	buf = malloc(3 * sizeof(char) * width * height);
	fread(buf, 1, 54, fp);
	unsigned int xy = fread(buf, 3 * sizeof(char), width * height, fp);
	if (xy != width * height) {
		cerr << "Wrong size " << xy;
		free(buf);
		exit(1);
	}
	fclose(fp);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
			GL_UNSIGNED_BYTE, buf);
	free(buf);
	return (texture);

}
void GL_callbacks::reshape(int w, int h) {
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, w, 0.0, h, -1.0, 1.0);
	glMatrixMode (GL_MODELVIEW);
}

void GL_callbacks::keypress(unsigned char key, int x, int y) {
	homerEmulator->keyEmulator.gl_key_press(key, x, y);
}
void GL_callbacks::keyrelease(unsigned char key, int x, int y) {
	homerEmulator->keyEmulator.gl_key_release(key, x, y);
}

void GL_callbacks::display(void) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	/*
	 glEnable (GL_TEXTURE_2D);
	 texture =
	 GL_callbacks::LoadBMPTexture(
	 "/wks/workspace/sandbox/GLUT-examples/resource/fpanel800x129x24b.bmp",
	 800, 129);
	 glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	 glTranslatef(0.0, 0.0, 0.0);
	 glColor3f(0.0, 0.0, 0.0);

	 glBegin (GL_POLYGON);

	 GLfloat left = -1.0f;
	 GLfloat right = 1.0f;
	 GLfloat up = 1.0f;
	 GLfloat down = -1.0f;
	 GLfloat z = 0.0f;
	 glTexCoord2f(0.0f, 0.0f); //Texture co-ordinate origin or lower left corner
	 glVertex3f(left, down, z); // v1 low-sx
	 glTexCoord2f(1.0f, 0.0f); //Texture co-ordinate for repeating image ten times form			      //origin to lower right corner
	 glVertex3f(right, down, z); // v2 low-dx
	 glTexCoord2f(1.0f, 1.0f); //repeat texture ten times form lower to top right corner.
	 glVertex3f(right, up, z); // v3 up-dx
	 glTexCoord2f(0.0f, 1.0f); //repeat texture ten time form top right to top left corner.
	 glVertex3f(left, up, z); // up-sx

	 glEnd();
	 */

	displayEmulator->draw();
	glDisable (GL_TEXTURE_2D); //Disable the texture
	glutSwapBuffers();

}

void GL_callbacks::timer(int value) {
	glutPostRedisplay();
	glutTimerFunc(homerEmulator->getRefreshRate(), GL_callbacks::timer, value);
}

HomerEmulator::HomerEmulator(WinstarEmulator * emulatedDisplay) {
	myargc = 1;
	myargv[0] = strdup("Homer Emulator");
	this->refreshRate = HEMUL_REFRESHRATE;
	callbacks.setHomerEmulator(this);
	callbacks.setDisplayEmulator(emulatedDisplay);
}

int HomerEmulator::start() {
	keyEmulator.gl_start();
	glutInit(&myargc, &myargv[0]);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

	glutInitWindowSize(800, 160);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Homer");
	glEnable (GL_DEPTH_TEST);
	glDepthMask (GL_TRUE);
	glDepthFunc (GL_LEQUAL);
	glShadeModel (GL_SMOOTH);
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE,
			GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	glutReshapeFunc(GL_callbacks::reshape);
	glutSetKeyRepeat (GLUT_KEY_REPEAT_OFF);
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

