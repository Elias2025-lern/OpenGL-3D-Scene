#version 330 core
layout (location = 0) in vec3 aPos;    // Vertex-Position
layout (location = 1) in vec3 aColor;  // Vertex-Farbe

out vec3 vertexColor; // Ausgabe-Farbe an den Fragment-Shader

// Neue Uniforms für die Transformationsmatrizen
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    // Wende die Matrizen in folgender Reihenfolge an: projection * view * model * position
    // OpenGL multipliziert Matrizen von rechts nach links
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    vertexColor = aColor; // Übergibt die Eingabefarbe direkt an die Ausgabe
}
