#version 330 core
layout (location = 0) in vec3 aPos;    // Vertex-Position
layout (location = 1) in vec3 aColor;  // Vertex-Farbe
layout (location = 2) in vec3 aNormal; // Vertex-Normale

out vec3 FragPos_world; // Fragment Position in Weltkoordinaten
out vec3 Normal_world;  // Normale in Weltkoordinaten
out vec3 VertexColor;   // Vertex-Farbe an den Fragment-Shader

// Neue Uniforms für die Transformationsmatrizen
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
// uniform mat3 normalMatrix; // Für korrekte Normalentransformation bei nicht-uniformer Skalierung

void main() {
    // Wende die Matrizen in folgender Reihenfolge an: projection * view * model * position
    // OpenGL multipliziert Matrizen von rechts nach links
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    FragPos_world = vec3(model * vec4(aPos, 1.0));
    // Normal_world = normalize(normalMatrix * aNormal); // Korrekte Methode
    Normal_world = normalize(mat3(transpose(inverse(model))) * aNormal); // Sicherste Methode
    VertexColor = aColor; // Übergibt die Eingabefarbe direkt an die Ausgabe
}
