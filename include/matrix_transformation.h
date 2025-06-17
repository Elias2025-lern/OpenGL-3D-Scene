#ifndef MATRIX_TRANS_H
#define MATRIX_TRANS_H


#include <math.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

//  4 × 4-Matrix in column-major order repräsentieren
void identity(GLfloat* out);

// die Matrix wird mit einer Translationsmatrix T(v) multipliziert 
// und das Ergebnis wird in out ablegt 
void translate(GLfloat* out, GLfloat* in, GLfloat* v);

// die Matrix wird mit einer Skalierungsmatrix S(v) multipliziert 
// und das Ergebnis wird in out ablegt
void scale(GLfloat* out, GLfloat* in, GLfloat* v);


//die Matrix wird mit einer Rotationsmatrix 𝑅𝑧(𝑎) (Rotation um die z-Achse mit Winkel a) multipliziert
// und das Ergebnis wird in out ablegt
void rotatez(GLfloat* out, GLfloat* in, GLfloat angle);


#endif