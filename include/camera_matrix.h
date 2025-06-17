#ifndef CAMERA_MATRIX_H
#define CAMERA_MATRIX_H

#include <GL/glew.h>

void lookAt(GLfloat* out, const GLfloat* eye, const GLfloat* center, const GLfloat* up);
void perspective(GLfloat* out, GLfloat fovy, GLfloat aspect, GLfloat near, GLfloat far);

#endif
