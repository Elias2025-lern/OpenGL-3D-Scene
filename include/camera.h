#ifndef CAMERA_H
#define CAMERA_H

#include <math.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// View-Matrix erstellen und in das Array *out* schreiben
void lookAt(GLfloat *out, const GLfloat *eye, const GLfloat *center, const GLfloat *up);
// Projektionsmatrix erstellen und in das Array *out* schreiben
void perspective(GLfloat *out, GLfloat fovy, GLfloat aspect, GLfloat near, GLfloat far);

#endif