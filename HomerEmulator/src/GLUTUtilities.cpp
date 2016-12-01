/*
 * GLUTutilities.cpp
 *
 *  Created on: 07/nov/2016
 *      Author: daniele
 */
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
      != width * height) {
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
