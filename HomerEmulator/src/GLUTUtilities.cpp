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
#include <GLUTUtilities.h>
#include <iostream>
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/loglevel.h>
#include <EmuGlobals.h>

using namespace std;
using namespace log4cplus;

namespace homeremulator {

GLuint GLUTUtilities::LoadBMPTexture(const char *filename, unsigned int width,
                                     unsigned int height) {
  GLuint texture;
  void *buf;
  int area;
  Logger logemu = Logger::getInstance(LOGEMULATOR);

  FILE* fp = fopen(filename, "r");
  if (!fp) {
    LOG4CPLUS_FATAL(logemu, "Cannot open " << filename);
    exit(1);
  }

  buf = malloc(3 * sizeof(char) * width * height);
  if (buf == nullptr) {
    LOG4CPLUS_FATAL(
        logemu, "Cannot alloc memory  " << (3 * sizeof(char) * width * height));
    exit(1);
  }

  fread(buf, 1, 54, fp); /* Skip first 54 bytes maybe an header :) */

  if ((area = fread(buf, 3 * sizeof(char), width * height, fp))
      != (int) (width * height)) {
    LOG4CPLUS_FATAL(
        logemu,
        "Wrong size Not enough data in bitmap " << area << " expected:"
            << width * height);
    free(buf);
    exit(1);
  }
  fclose(fp);

  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 129, 0, GL_RGB, GL_UNSIGNED_BYTE,
               buf);
  free(buf);
  return (texture);

}
}
