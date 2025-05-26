#version 330 core
out vec4 FragColor;

in vec3 vertexColor; // Input color, interpolated from vertex shader

void main() {
    FragColor = vec4(vertexColor, 1.0f); // Use the interpolated color as the final fragment color
}