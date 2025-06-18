// light.h – Definition der Lichtstruktur und Funktion zur Übergabe an den Shader
#ifndef LIGHT_H
#define LIGHT_H

#include "shader.h"
#include "matrix.h" // Für vec3

// Struktur zur Beschreibung einer Lichtquelle
typedef struct {
    vec3 position;            // Position der Lichtquelle im Raum
    vec3 color;               // Lichtfarbe (z. B. weiß)
    float ambient_strength;   // Stärke des ambienten Lichts
    float specular_strength;  // Stärke der Spiegelung
} Light;

// Funktion zum Setzen der Licht-Uniforms im Shader
void light_set_uniforms(Shader* shader, Light* light, vec3 viewPos);

#endif
