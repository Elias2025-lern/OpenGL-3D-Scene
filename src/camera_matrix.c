// camera_matrix.c – Enthält die Implementierung von lookAt und perspective
#include <math.h>
#include <string.h> // für memcpy

// Funktion zur Erstellung einer View-Matrix (Kamerablickrichtung)
void lookAt(GLfloat* out, const GLfloat* eye, const GLfloat* center, const GLfloat* up) {
    float f[3], s[3], u[3];

    // Richtungsvektor berechnen (Ziel - Auge)
    for (int i = 0; i < 3; i++) f[i] = center[i] - eye[i];

    // Normalisieren f
    float len = sqrtf(f[0] * f[0] + f[1] * f[1] + f[2] * f[2]);
    for (int i = 0; i < 3; i++) f[i] /= len;

    // Kreuzprodukt s = f x up (seitliche Richtung)
    s[0] = f[1] * up[2] - f[2] * up[1];
    s[1] = f[2] * up[0] - f[0] * up[2];
    s[2] = f[0] * up[1] - f[1] * up[0];

    // Normalisieren s
    len = sqrtf(s[0] * s[0] + s[1] * s[1] + s[2] * s[2]);
    for (int i = 0; i < 3; i++) s[i] /= len;

    // u = s x f (neue up-Richtung)
    u[0] = s[1] * f[2] - s[2] * f[1];
    u[1] = s[2] * f[0] - s[0] * f[2];
    u[2] = s[0] * f[1] - s[1] * f[0];

    // View-Matrix zusammenbauen (Spaltenweise)
    out[0] = s[0]; out[4] = s[1]; out[8] = s[2];  out[12] = -(s[0] * eye[0] + s[1] * eye[1] + s[2] * eye[2]);
    out[1] = u[0]; out[5] = u[1]; out[9] = u[2];  out[13] = -(u[0] * eye[0] + u[1] * eye[1] + u[2] * eye[2]);
    out[2] = -f[0]; out[6] = -f[1]; out[10] = -f[2]; out[14] = +(f[0] * eye[0] + f[1] * eye[1] + f[2] * eye[2]);
    out[3] = 0.0f; out[7] = 0.0f; out[11] = 0.0f;  out[15] = 1.0f;
}

// Funktion zur Erstellung einer Projektionsmatrix (Perspektive)
void perspective(GLfloat* out, GLfloat fovy, GLfloat aspect, GLfloat near, GLfloat far) {
    float f = 1.0f / tanf(fovy / 2.0f);
    memset(out, 0, sizeof(float) * 16);
    out[0] = f / aspect;
    out[5] = f;
    out[10] = (far + near) / (near - far);
    out[11] = -1.0f;
    out[14] = (2.0f * far * near) / (near - far);
}
