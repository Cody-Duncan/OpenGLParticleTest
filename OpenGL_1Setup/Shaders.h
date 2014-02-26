#pragma once

#include <glload/gl_all.h>
#include <glload/gl_load.h>

GLint CreateShader(const char * filename, GLenum type);
void LinkProgram(GLuint program, GLuint vertexShader, GLuint pixelShader);
void printActiveUniforms();

