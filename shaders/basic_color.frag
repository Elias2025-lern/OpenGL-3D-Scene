#version 330 core
out vec4 FragColor;

in vec3 vertexColor; // Eingabefarbe, vom Vertex-Shader interpoliert

void main() {
    FragColor = vec4(vertexColor, 1.0f); // Verwende die interpolierte Farbe als finale Fragmentfarbe
}
