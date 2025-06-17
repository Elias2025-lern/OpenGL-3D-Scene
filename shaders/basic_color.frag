#version 330 core

// Ausgabe: Endgültige Farbe
out vec4 FragColor;

// Eingaben vom Vertex-Shader
in vec2 TexCoord;    // Texturkoordinaten
in vec3 FragPos;     // Fragmentposition im Weltraum
in vec3 Normal;      // Normalenvektor

// Uniform-Variablen vom Hauptprogramm
uniform sampler2D texture1;  // Textur-Sampler
uniform vec3 lightPos;       // Lichtposition im Weltraum
uniform vec3 viewPos;        // Kameraposition
uniform vec3 lightColor;     // Lichtfarbe
uniform bool isBackground;   // Flag für Hintergrundobjekte

void main()
{
    // Falls es ein Hintergrundobjekt ist, nur Textur ohne Beleuchtung anzeigen
    if (isBackground) {
        vec3 bg = texture(texture1, TexCoord).rgb * 0.3; // Hintergrund abdunkeln
        FragColor = vec4(bg, 1.0);
        return;
    }

    // Beleuchtungsberechnungen
    vec3 norm = normalize(Normal);               // Normalenvektor normalisieren
    vec3 lightDir = normalize(lightPos - FragPos); // Lichtrichtungsvektor

    // Ambientale Beleuchtung (Grundlicht)
    float ambientStrength = 0.5;
    vec3 ambient = ambientStrength * lightColor;

    // Diffuse Beleuchtung (Streulicht)
    float diff = max(dot(norm, lightDir), 0.0);  // Lambertsches Kosinusgesetz
    vec3 diffuse = diff * lightColor;

    // Spiegelnede Beleuchtung (Glanzlichter)
    float specularStrength = 7.0;
    vec3 viewDir = normalize(viewPos - FragPos); // Blickrichtung
    vec3 reflectDir = reflect(-lightDir, norm);  // Reflektionsvektor
    // Phong-Beleuchtungsmodell (32 ist der Glanzwert)
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    // Alle Lichtkomponenten kombinieren
    vec3 lighting = ambient + diffuse + specular;

    // Farbe aus der Textur holen
    vec3 texColor = texture(texture1, TexCoord).rgb;

    // Endgültige Farbe mit Beleuchtung
    FragColor = vec4(lighting * texColor, 1.0);
}