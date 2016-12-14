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

#ifndef HOMEREMULATOR_HPP_
#define HOMEREMULATOR_HPP_
#include <EmulatedHw.hpp>
#include <GL/glut.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <KeyEmulator.hpp>
#include <Scheduler.hpp>
#include <WinstarEmulator.hpp>
#include "GLUTUtilities.h"

#define HEMUL_REFRESHRATE 250 // milliseconds

using namespace homerio;

namespace homeremulator {
class HomerEmulator;

struct GL_staticCall {
  static void reshape(int w, int h);
  static void keypress(unsigned char key, int x, int y);
  static void keyrelease(unsigned char key, int x, int y);
  static void display();
  static void timer(int value);
  static void idle();

  static GLuint LoadBMPTexture(const char *filename, unsigned int width,
                               unsigned int height);
  void setHomerEmulator(HomerEmulator* e) {
    GL_staticCall::homerEmulator = e;
  }
  void setDisplayEmulator(WinstarEmulator* d) {
    GL_staticCall::displayEmulator = d;
  }
  static HomerEmulator *homerEmulator;
  static WinstarEmulator *displayEmulator;

};

class HomerEmulator {
 private:
  char *myargv[1];
  int myargc;
  unsigned int refreshRate;

 public:
  KeyEmulator keyEmulator;
  GL_staticCall GL_calls;
  void makeRasterFont(void);

  const string & getKeyEventFilename() {
    return keyEmulator.getEvent();
  }

 public:

  HomerEmulator(WinstarEmulator *emulatedDisplay);
  virtual ~HomerEmulator() {
  }

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
