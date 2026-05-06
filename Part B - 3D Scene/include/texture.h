#pragma once
#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"
#include <iostream>

GLuint setup_texture(const char* filename)
{
	int w, h, chan;
	GLuint texObject;
	glGenTextures(1, &texObject);
	glBindTexture(GL_TEXTURE_2D, texObject);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	stbi_set_flip_vertically_on_load(true);
	unsigned char* pixels = stbi_load(filename, &w, &h, &chan, STBI_rgb_alpha);
	if (pixels) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	}
	delete[] pixels;
	glGenerateMipmap(GL_TEXTURE_2D);
	return texObject;
}

GLuint setup_mipmaps(const char* filename[], int n)
{
	int w[16], h[16], chan[16];
	unsigned char* pixels[16];
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	GLuint texObject;
	glGenTextures(1, &texObject);
	glBindTexture(GL_TEXTURE_2D, texObject);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	stbi_set_flip_vertically_on_load(true);
	for (int i = 0; i < n; i++) {
		pixels[i] = stbi_load(filename[i], &w[i], &h[i], &chan[i], 0);
		if (pixels[i]) {
			glTexImage2D(GL_TEXTURE_2D, i, GL_RGBA, w[i], h[i], 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels[i]);
			delete[] pixels;
			glDisable(GL_TEXTURE_2D);
			glDisable(GL_BLEND);
			return texObject;
		}
	}
	return 0;
}
