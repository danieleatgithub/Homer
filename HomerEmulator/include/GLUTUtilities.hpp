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

#ifndef GLUTUTILITIES_H_
#define GLUTUTILITIES_H_
#include <GL/freeglut.h>

namespace homeremulator {
/**
 * Static utilities
 */
class GLUTUtilities {
 public:
  /**
   * Load a bmp file as a texture and the two-dimensional texture image
   * terminate on fail
   * @param filename file to load
   * @param width
   * @param height
   * @return texture ID loaded
   */
  const static GLuint
  LoadBMPTexture(const char *filename, const unsigned int width,
                 const unsigned int height);
};

}
#endif /* GLUTUTILITIES_H_ */
