#version 330 core
out vec4 FragColor;

in vec3 FragPos_world;
in vec3 Normal_world;
in vec3 VertexColor; // Eingabefarbe, vom Vertex-Shader interpoliert

uniform vec3 lightPos_world; // Position der Lichtquelle in Weltkoordinaten
uniform vec3 viewPos_world;  // Position des Betrachters/der Kamera in Weltkoordinaten
uniform vec3 lightColor;     // Farbe der Lichtquelle
// uniform vec3 objectColor; // Alternative: Objektfarbe als Uniform

void main() {
    // Umgebungslicht (Ambient)
    float ambientStrength = 0.1; // Stärke des Umgebungslichts
    vec3 ambient = ambientStrength * lightColor;

    // Diffuses Licht
    vec3 norm = normalize(Normal_world);
    vec3 lightDir = normalize(lightPos_world - FragPos_world);
    float diff = max(dot(norm, lightDir), 0.0); // Intensität des diffusen Lichts
    vec3 diffuse = diff * lightColor;

    // Spiegelndes Licht (Specular)
    float specularStrength = 0.5; // Stärke des spiegelnden Lichts
    vec3 viewDir = normalize(viewPos_world - FragPos_world);
    vec3 reflectDir = reflect(-lightDir, norm); // Reflektierter Lichtvektor
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32); // 32 ist der Glanzfaktor (shininess)
    vec3 specular = specularStrength * spec * lightColor;

    // Kombiniere die Komponenten mit der Objektfarbe (hier VertexColor)
    vec3 result = (ambient + diffuse + specular) * VertexColor;
    FragColor = vec4(result, 1.0f);
}
