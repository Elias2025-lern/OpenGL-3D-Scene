// object.h
#ifndef OBJECT_H
#define OBJECT_H

#include <GL/glew.h>
#include "matrix.h"
#include "shader.h"

// Struktur für ein darstellbares Objekt
typedef struct {
    GLuint VAO;
    GLuint VBO;
    int vertex_count;
    mat4 model_matrix;
} RenderObject;

// Erstellt ein Objekt mit den angegebenen Vertexdaten
RenderObject object_create(const float* vertices, int num_floats, int vertex_size);

// Zeichnet das Objekt mit den angegebenen Matrizen
void object_draw(RenderObject* obj, const Shader* shader_program, const mat4* view, const mat4* projection);

// Gibt die Ressourcen des Objekts frei
void object_destroy(RenderObject* obj);

#endif // OBJECT_H
