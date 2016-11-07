/*
 * GLUTUtilities.h
 *
 *  Created on: 07/nov/2016
 *      Author: daniele
 */

#ifndef GLUTUTILITIES_H_
#define GLUTUTILITIES_H_
#include <GL/freeglut.h>

class GLUTUtilities {
public:
	static GLuint
	LoadBMPTexture(const char *filename, unsigned int width,
			unsigned int height);
};

#endif /* GLUTUTILITIES_H_ */
