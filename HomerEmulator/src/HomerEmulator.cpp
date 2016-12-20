/*******************************************************************************
 * Copyright (C) 2016  Daniele Colombo
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *******************************************************************************/
#include <HomerEmulator.hpp>
#include <GL/freeglut.h>
#include <stdlib.h>
#include <string.h>

namespace homeremulator {

HomerEmulator* GL_staticCall::homerEmulator = nullptr;
WinstarEmulator* GL_staticCall::displayEmulator = nullptr;

void GL_staticCall::reshape(const int w, const int h) {
}

void GL_staticCall::keypress(const unsigned char key, const int x,
                             const int y) {
  homerEmulator->keyEmulator.gl_key_press(key, x, y);
}
void GL_staticCall::keyrelease(const unsigned char key, const int x,
                               const int y) {
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

  // Rectangle for the background texture
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
  // Rectangle for the background texture

  // TODO: Add a draw observer
  displayEmulator->draw();  // Display emulator

  glFlush();

}

/**
 * User timer for ciclic refresh
 * @param value
 */
void GL_staticCall::timer(const int value) {
  glutPostRedisplay();
  glutTimerFunc(homerEmulator->getRefreshRate(), GL_staticCall::timer, value);
}

HomerEmulator::HomerEmulator(WinstarEmulator* const emulatedDisplay) {
  myargc = 1;
  myargv[0] = strdup("Homer Emulator");
  this->refreshRate = HEMUL_REFRESHRATE;
  GL_calls.setHomerEmulator(this);
  GL_calls.setDisplayEmulator(emulatedDisplay);
}
HomerEmulator::~HomerEmulator() {
  free (myargv[0]);
}

void HomerEmulator::start() {
  unsigned int witdh = 800;
  unsigned int height = 129;

  keyEmulator.gl_start();

  glutInit(&myargc, &myargv[0]);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

  glutInitWindowSize(witdh, height);
  glutInitWindowPosition(0, 0);
  glutCreateWindow (myargv[0]);
  glEnable (GL_DEPTH_TEST);
  glDepthMask (GL_TRUE);
  glDepthFunc (GL_LEQUAL);
  glShadeModel (GL_SMOOTH);
  glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

  GLUTUtilities::LoadBMPTexture(
      "/wks/workspace/sandbox/GLUT-examples/resource/fpanel800x129x24b.bmp",
      witdh, height);

  glutReshapeFunc(GL_staticCall::reshape);
  glutSetKeyRepeat (GLUT_KEY_REPEAT_OFF);
  glutKeyboardFunc(GL_staticCall::keypress);
  glutKeyboardUpFunc(GL_staticCall::keyrelease);
  glutDisplayFunc(GL_staticCall::display);
  glutTimerFunc(0, GL_staticCall::timer, 0);

}

void HomerEmulator::stop() {
  keyEmulator.gl_stop();
}

void HomerEmulator::mainLoop() {
  glutMainLoop();
}

}

