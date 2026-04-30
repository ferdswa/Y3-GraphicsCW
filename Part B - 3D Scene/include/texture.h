#pragma once
#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"
#include <iostream>

GLuint setup_texture(const char* filename)
{
	int w, h, chan;
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	GLuint texObject;
	glGenTextures(1, &texObject);
	glBindTexture(GL_TEXTURE_2D, texObject);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	stbi_set_flip_vertically_on_load(true);
	unsigned char* pixels = stbi_load(filename, &w, &h, &chan, 0);
	if (pixels) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
		delete[] pixels;
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);
		return texObject;
	}
	return 0;
}

GLuint setup_mipmaps(const char* filename[], int n)
{
	return 0;
}
