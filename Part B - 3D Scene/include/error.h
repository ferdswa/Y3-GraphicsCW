#pragma once
#include "shader.h"
#include <iostream>

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include "file.h"

void APIENTRY DebugCallback(GLenum source, GLenum type, GLenum id, GLenum severity, GLenum length, const GLchar* message, const void* userParam) {
	if (type == GL_DEBUG_TYPE_ERROR)
		fprintf(stderr, "GL ERROR - %s\n", message);
}