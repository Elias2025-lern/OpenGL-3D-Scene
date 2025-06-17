#include "matrix_transformation.h"

void identity(GLfloat* out) {
    for (int i = 0; i < 16; i++) {
        if (i%5 == 0) {
            out[i] = 1.0f;
        } else { 
            out[i] = 0.0f;
        }
    }
}

void translate(GLfloat* out, GLfloat* in, GLfloat* v) {
    for (int i = 0; i < 16; i++) {
        out[i] = in[i];
    }

    for (int i = 0; i < 4; i++) {
        out[i + 3*4] = in[i + 0 * 4] * v[0] +
                        in[i + 1 * 4] * v[1] +
                        in[i + 2 * 4] * v[2] +
                        in[i + 3 * 4];
    }
}

void scale(GLfloat* out, GLfloat* in, GLfloat* v) {
    //x-Achse skalieren
    for (int i = 0; i < 4; ++i) {
        out[i + 0 * 4] = in[i + 0 * 4] * v[0];
    }

    //y-Achse skalieren
    for (int i = 0; i < 4; ++i) {
        out[i + 1 * 4] = in[i + 1 * 4] * v[1];
    }

    //z-Achse skalieren
    for (int i = 0; i < 4; ++i) {
        out[i + 2 * 4] = in[i + 2 * 4] * v[2];
    }

    //Letzte Spalte bleibt unverändert
    for (int i = 0; i < 4; ++i) {
        out[i + 3 * 4] = in[i + 3 * 4];
    }
}


void rotatez(GLfloat* out, GLfloat* in, GLfloat angle) {
    GLfloat c = cosf(angle);
    GLfloat s = sinf(angle);

    GLfloat tmp[16];

    // x-Achse rotiert
    for (int i = 0; i < 4; ++i) {
        tmp[i + 0 * 4] = in[i + 0 * 4] * c + in[i + 1 * 4] * s;
    }

    // y-Achse rotiert
    for (int i = 0; i < 4; ++i) {
        tmp[i + 1 * 4] = in[i + 1 * 4] * c - in[i + 0 * 4] * s;
    }

    // z-Achse und letze Spalte 3 bleiben gleich
    for (int j = 2; j < 4; ++j) {
        for (int i = 0; i < 4; ++i) {
            tmp[i + j * 4] = in[i + j * 4];
        }
    }
    // Ergebnis in out kopieren
    for (int i = 0; i < 16; ++i) {
        out[i] = tmp[i];
    }
}