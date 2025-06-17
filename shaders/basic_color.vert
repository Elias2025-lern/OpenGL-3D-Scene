#version 330 core

// Eingaben vom Mesh (aus dem VAO)
layout (location = 0) in vec3 aPos;         // Position des Vertex
layout (location = 1) in vec2 aTexCoord;    // Texturkoordinaten
layout (location = 2) in vec3 aNormal;      // Normalenvektor

// Weitergabe an den Fragment-Shader
out vec2 TexCoord;
out vec3 FragPos;        // Weltposition des Fragments
out vec3 Normal;         // Normalenvektor im Weltkoordinatensystem

// Uniforms für Transformationen
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    // Umwandlung der lokalen Position in Clip-Space
    gl_Position = projection * view * model * vec4(aPos, 1.0);

    // Weltposition berechnen
    FragPos = vec3(model * vec4(aPos, 1.0));

    // Normalenvektor transformieren
    Normal = mat3(transpose(inverse(model))) * aNormal;

    // Texturkoordinaten weitergeben
    TexCoord = aTexCoord;
}
