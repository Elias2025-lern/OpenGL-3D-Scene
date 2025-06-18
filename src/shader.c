#include "shader.h"
#include <stdio.h>  // Für fprintf
#include <stdlib.h> // Für free (für aus Datei gelesene Strings)

// Hilfsfunktion, um einen einzelnen Shader (Vertex oder Fragment) zu kompilieren
static GLuint compile_single_shader(GLenum type, const char *source)
{
    assert(source != NULL && "Shader-Quelle darf nicht NULL sein");

    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        fprintf(stderr, "Shader-Kompilierungsfehler (%s): %s\n",
                (type == GL_VERTEX_SHADER ? "Vertex" : "Fragment"), infoLog);
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

// Funktion, um ein Shader-Programm aus Vertex- und Fragment-Shader-Dateien zu erstellen
Shader shader_create(const char *vertexPath, const char *fragmentPath)
{
    Shader s = {.id = 0}; // Initialisiere Shader mit ID 0

    // 1. Lese Shader-Quellcode aus Dateien
    char *vertexCode = read_file(vertexPath);
    char *fragmentCode = read_file(fragmentPath);

    if (vertexCode == NULL || fragmentCode == NULL)
    {
        // Fehlermeldungen wurden bereits von read_file ausgegeben
        if (vertexCode)
            free(vertexCode);
        if (fragmentCode)
            free(fragmentCode);
        return s; // Ungültigen Shader zurückgeben
    }

    // 2. Shader kompilieren
    GLuint vertexShader = compile_single_shader(GL_VERTEX_SHADER, vertexCode);
    GLuint fragmentShader = compile_single_shader(GL_FRAGMENT_SHADER, fragmentCode);

    // Speicher für Shader-Quellcode nach der Kompilierung freigeben
    free(vertexCode);
    free(fragmentCode);

    if (vertexShader == 0 || fragmentShader == 0)
    {
        // Fehlermeldungen wurden bereits von compile_single_shader ausgegeben
        if (vertexShader)
            glDeleteShader(vertexShader);
        if (fragmentShader)
            glDeleteShader(fragmentShader);
        return s; // Ungültigen Shader zurückgeben
    }

    // 3. Shader-Programm verlinken
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
        fprintf(stderr, "Fehler beim Verlinken des Shader-Programms: %s\n", infoLog);
        glDeleteProgram(s.id);
        s.id = 0; // Als ungültig markieren
    }

    // Einzelne Shader löschen, nachdem sie in das Programm verlinkt wurden
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return s;
}

// Funktion, um das Shader-Programm zu aktivieren (zu benutzen)
void shader_use(const Shader* shader)
{
    assert(shader != NULL && "Shader-Zeiger darf nicht NULL sein");
    glUseProgram(shader->id);
}

// Funktion, um eine float-Uniform im Shader zu setzen
void shader_set_float(Shader *shader, const char *name, float value)
{
    assert(shader != NULL && "Shader-Zeiger darf nicht NULL sein");
    assert(name != NULL && "Uniform-Name darf nicht NULL sein");
    glUniform1f(glGetUniformLocation(shader->id, name), value);
}

// Funktion, um eine int-Uniform im Shader zu setzen
void shader_set_int(Shader *shader, const char *name, int value)
{
    assert(shader != NULL && "Shader-Zeiger darf nicht NULL sein");
    assert(name != NULL && "Uniform-Name darf nicht NULL sein");
    glUniform1i(glGetUniformLocation(shader->id, name), value);
}

// Funktion, um das Shader-Programm zu löschen
void shader_destroy(Shader *shader)
{
    assert(shader != NULL && "Shader-Zeiger darf nicht NULL sein");
    glDeleteProgram(shader->id);
    shader->id = 0; // Als freigegeben markieren
}
