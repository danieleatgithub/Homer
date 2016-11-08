/*
 * GLUTutilities.cpp
 *
 *  Created on: 07/nov/2016
 *      Author: daniele
 */
#include <GLUTUtilities.h>
#include <iostream>
#include <stdexcept>

GLuint GLUTUtilities::LoadBMPTexture(const char *filename, unsigned int width,
		unsigned int height) {
	GLuint texture;
	void *buf;
	FILE* fp = fopen(filename, "r");
	if (!fp) {
		throw std::invalid_argument("Cannot open bitmap");
	}

	buf = malloc(3 * sizeof(char) * width * height);

	fread(buf, 1, 54, fp); // Skip header

	if (fread(buf, 3 * sizeof(char), width * height, fp) != width * height) {
		free(buf);
		throw std::invalid_argument("Not enough data in bitmap");
	}
	fclose(fp);

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 129, 0, GL_RGB,
			GL_UNSIGNED_BYTE, buf);
	free(buf);
	return (texture);

}

