#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <stdbool.h>
#include "utils.h" // To use read_file

// Define Shader struct to encapsulate the shader program ID
typedef struct
{
    GLuint id;
} Shader;

// Function to create a shader program from vertex and fragment shader files
Shader shader_create(const char *vertexPath, const char *fragmentPath);

// Function to activate (use) the shader program
void shader_use(Shader *shader);

// Function to set a float uniform in the shader
void shader_set_float(Shader *shader, const char *name, float value);

// Function to set an int uniform in the shader
void shader_set_int(Shader *shader, const char *name, int value);

// Function to delete the shader program
void shader_destroy(Shader *shader);

#endif // SHADER_H