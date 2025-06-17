#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <stdbool.h>
#include "utils.h" // Um read_file zu verwenden

// Definiert die Shader-Struktur, die die ID des Shader-Programms kapselt
typedef struct
{
    GLuint id;
} Shader;

// Funktion zum Erstellen eines Shader-Programms aus Vertex- und Fragment-Shader-Dateien
Shader shader_create(const char *vertexPath, const char *fragmentPath);

// Funktion zum Aktivieren (Benutzen) des Shader-Programms
void shader_use(const Shader* shader);

// Funktion zum Setzen einer float-Uniform-Variable im Shader
void shader_set_float(Shader *shader, const char *name, float value);

// Funktion zum Setzen einer int-Uniform-Variable im Shader
void shader_set_int(Shader *shader, const char *name, int value);

// Funktion zum Löschen des Shader-Programms
void shader_destroy(Shader *shader);

#endif // SHADER_H
