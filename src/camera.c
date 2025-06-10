#include "camera.h"


void lookAt(GLfloat *out, const GLfloat *eye, const GLfloat *center, const GLfloat *up) {
    // Forward-Vektor berechnen und normalisieren
    GLfloat fx = center[0] - eye[0];
    GLfloat fy = center[1] - eye[1];
    GLfloat fz = center[2] - eye[2];
    GLfloat f_len = sqrt(fx*fx + fy*fy + fz*fz);
    fx /= f_len;
    fy /= f_len;
    fz /= f_len;

    // Seiten-Vektor berechnen (x-Achse) = f × up
    GLfloat sx = fy * up[2] - fz * up[1];
    GLfloat sy = fz * up[0] - fx * up[2];
    GLfloat sz = fx * up[1] - fy * up[0];
    GLfloat s_len = sqrt(sx*sx + sy*sy + sz*sz);
    sx /= s_len;
    sy /= s_len;
    sz /= s_len;
    // korrigierter Up-Vektor (y-Achse) = s × f
    GLfloat ux = sy * fz - sz * fy;
    GLfloat uy = sz * fx - sx * fz;
    GLfloat uz = sx * fy - sy * fx;

    // View-Matrix in Column-Major-Format
    out[0] = sx;
    out[1] = ux;
    out[2] = -fx;
    out[3] = 0.0f;
    out[4] = sy;
    out[5] = uy;
    out[6] = -fy;
    out[7] = 0.0f;

    out[8]  = sz;
    out[9]  = uz;
    out[10] = -fz;
    out[11] = 0.0f;

    out[12] = - (sx * eye[0] + sy * eye[1] + sz * eye[2]);
    out[13] = - (ux * eye[0] + uy * eye[1] + uz * eye[2]);
    out[14] =    (fx * eye[0] + fy * eye[1] + fz * eye[2]);
    out[15] = 1.0f;
}

void perspective(GLfloat *out, GLfloat fovy, GLfloat aspect, GLfloat near, GLfloat far) {
    GLfloat tanHalfFovy = tanf(fovy / 2.0f);
    for (int i = 0; i < 16; i ++) {
        out[i] = 0.0f;
    }
    out[0]  = 1.0f / (aspect * tanHalfFovy);                         
    out[5]  = 1.0f / tanHalfFovy;                               
    out[10] = -(far + near) / (far - near);  
    out[11] = -2.0f * far * near / (far - near);                        
    out[14] = -1.0f;                                               
}