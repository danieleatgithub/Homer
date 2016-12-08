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
#include <memory>
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/loglevel.h>
#include <HomerEmulator.hpp>
#include <EmulatedHw.hpp>

using namespace std;
using namespace homerio;
using namespace log4cplus;

namespace homeremulator {

GLuint fontOffset;
GLuint texture;
GLfloat white[3] = { 1.0, 1.0, 1.0 };
GLfloat green[3] = { 0.0, 1.0, 0.0 };
GLfloat black[3] = { 0.0, 0.0, 0.0 };
GLfloat grey[3] = { 0.5, 0.5, 0.5 };
GLfloat lightgrey[3] = { 0.9, 0.9, 0.9 };
HomerEmulator* GL_staticCall::homerEmulator = nullptr;
WinstarEmulator* GL_staticCall::displayEmulator = nullptr;

void GL_staticCall::reshape(int w, int h) {
}

void GL_staticCall::keypress(unsigned char key, int x, int y) {
  homerEmulator->keyEmulator.gl_key_press(key, x, y);
}
void GL_staticCall::keyrelease(unsigned char key, int x, int y) {
  homerEmulator->keyEmulator.gl_key_release(key, x, y);
}

void GL_staticCall::display(void) {

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable (GL_TEXTURE_2D);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
  glTranslatef(0.0, 0.0, 0.0);
  glColor3f(0.0, 0.0, 0.0);

  GLfloat left;
  GLfloat right;
  GLfloat up;
  GLfloat down;

  glBegin (GL_POLYGON);
  left = -1.0f;
  right = 1.0f;
  up = 1.0f;
  down = -1.0f;
  glTexCoord2f(0.0f, 0.0f);  //Texture co-ordinate origin or lower left corner
  glVertex3f(left, down, 0.0f);  // v1 low-sx
  glTexCoord2f(1.0f, 0.0f);  //Texture co-ordinate for repeating image ten times form			      //origin to lower right corner
  glVertex3f(right, down, 0.0f);  // v2 low-dx
  glTexCoord2f(1.0f, 1.0f);  //repeat texture ten times form lower to top right corner.
  glVertex3f(right, up, 0.0f);  // v3 up-dx
  glTexCoord2f(0.0f, 1.0f);  //repeat texture ten time form top right to top left corner.
  glVertex3f(left, up, 0.0f);  // up-sx
  glEnd();
  glDisable(GL_TEXTURE_2D);  //Disable the texture
  displayEmulator->draw();
  glFlush();

}

void GL_staticCall::timer(int value) {
  glutPostRedisplay();
  glutTimerFunc(homerEmulator->getRefreshRate(), GL_staticCall::timer, value);
}

HomerEmulator::HomerEmulator(WinstarEmulator * emulatedDisplay) {
  myargc = 1;
  myargv[0] = strdup("Homer Emulator");
  this->refreshRate = HEMUL_REFRESHRATE;
  GL_calls.setHomerEmulator(this);
  GL_calls.setDisplayEmulator(emulatedDisplay);
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
  glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

  GLUTUtilities::LoadBMPTexture(
      "/wks/workspace/sandbox/GLUT-examples/resource/fpanel800x129x24b.bmp",
      800, 129);

  glutReshapeFunc(GL_staticCall::reshape);
  glutSetKeyRepeat (GLUT_KEY_REPEAT_OFF);
  glutKeyboardFunc(GL_staticCall::keypress);
  glutKeyboardUpFunc(GL_staticCall::keyrelease);
  glutDisplayFunc(GL_staticCall::display);
  glutTimerFunc(0, GL_staticCall::timer, 0);

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

