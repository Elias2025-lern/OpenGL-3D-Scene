#include "shader.h"
#include <stdio.h>  // For fprintf
#include <stdlib.h> // For free (for string read from file)

// Helper function to compile a single shader (vertex or fragment)
static GLuint compile_single_shader(GLenum type, const char *source)
{
    assert(source != NULL && "Shader source cannot be NULL");

    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        fprintf(stderr, "Shader compilation error (%s): %s\n",
                (type == GL_VERTEX_SHADER ? "Vertex" : "Fragment"), infoLog);
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

// Function to create a shader program from vertex and fragment shader files
Shader shader_create(const char *vertexPath, const char *fragmentPath)
{
    Shader s = {.id = 0}; // Initialize shader with ID 0

    // 1. Read shader source code from files
    char *vertexCode = read_file(vertexPath);
    char *fragmentCode = read_file(fragmentPath);

    if (vertexCode == NULL || fragmentCode == NULL)
    {
        // Error messages are already printed by read_file
        if (vertexCode)
            free(vertexCode);
        if (fragmentCode)
            free(fragmentCode);
        return s; // Return invalid shader
    }

    // 2. Compile shaders
    GLuint vertexShader = compile_single_shader(GL_VERTEX_SHADER, vertexCode);
    GLuint fragmentShader = compile_single_shader(GL_FRAGMENT_SHADER, fragmentCode);

    // Free memory allocated for shader source code after compilation
    free(vertexCode);
    free(fragmentCode);

    if (vertexShader == 0 || fragmentShader == 0)
    {
        // Error messages are already printed by compile_single_shader
        if (vertexShader)
            glDeleteShader(vertexShader);
        if (fragmentShader)
            glDeleteShader(fragmentShader);
        return s; // Return invalid shader
    }

    // 3. Link shader program
    s.id = glCreateProgram();
    glAttachShader(s.id, vertexShader);
    glAttachShader(s.id, fragmentShader);
    glLinkProgram(s.id);

    int success;
    char infoLog[512];
    glGetProgramiv(s.id, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(s.id, 512, NULL, infoLog);
        fprintf(stderr, "Shader program linking error: %s\n", infoLog);
        glDeleteProgram(s.id);
        s.id = 0; // Mark as invalid
    }

    // Delete the individual shaders after they've been linked into the program
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return s;
}

// Function to activate (use) the shader program
void shader_use(Shader *shader)
{
    assert(shader != NULL && "Shader pointer cannot be NULL");
    glUseProgram(shader->id);
}

// Function to set a float uniform in the shader
void shader_set_float(Shader *shader, const char *name, float value)
{
    assert(shader != NULL && "Shader pointer cannot be NULL");
    assert(name != NULL && "Uniform name cannot be NULL");
    glUniform1f(glGetUniformLocation(shader->id, name), value);
}

// Function to set an int uniform in the shader
void shader_set_int(Shader *shader, const char *name, int value)
{
    assert(shader != NULL && "Shader pointer cannot be NULL");
    assert(name != NULL && "Uniform name cannot be NULL");
    glUniform1i(glGetUniformLocation(shader->id, name), value);
}

// Function to delete the shader program
void shader_destroy(Shader *shader)
{
    assert(shader != NULL && "Shader pointer cannot be NULL");
    glDeleteProgram(shader->id);
    shader->id = 0; // Mark as freed
}