#ifndef MATRIX_H
#define MATRIX_H

// --- Definition der Struktur für einen 3D-Vektor (vec3) ---
typedef struct
{
    float x, y, z;
} vec3;

// --- Definition der Struktur für einen 4D-Vektor (vec4) ---
typedef struct
{
    float x, y, z, w;
} vec4;

// --- Definition der Struktur für eine 4x4-Matrix (mat4) ---
// Speicherung in Spalten-Hauptordnung (column-major) - Standard bei OpenGL
// m[Spalte][Zeile]
typedef struct
{
    float m[4][4];
} mat4;

// --- Deklaration der mathematischen Funktionen für Vektoren ---

// Erzeugt einen neuen vec3
vec3 vec3_create(float x, float y, float z);
// Erzeugt einen neuen vec4
vec4 vec4_create(float x, float y, float z, float w);

// Addition von zwei vec3
vec3 vec3_add(vec3 a, vec3 b);
// Subtraktion von zwei vec3
vec3 vec3_sub(vec3 a, vec3 b);
// Skalierung eines vec3 mit einem Skalar
vec3 vec3_scale(vec3 v, float s);
// Skalarprodukt (Dot-Produkt) von zwei vec3
float vec3_dot(vec3 a, vec3 b);
// Vektorprodukt (Cross-Produkt) von zwei vec3
vec3 vec3_cross(vec3 a, vec3 b);
// Normalisierung eines vec3 (auf Länge 1 bringen)
vec3 vec3_normalize(vec3 v);
// Berechnung der Länge eines vec3
float vec3_length(vec3 v);

// Umwandlung von vec3 zu vec4 (w = 1.0f für Punkt, 0.0f für Richtungsvektor)
vec4 vec3_to_vec4(vec3 v, float w);
// Umwandlung von vec4 zu vec3
vec3 vec4_to_vec3(vec4 v);

// --- Deklaration der mathematischen Funktionen für Matrizen ---

// Erzeugt eine Einheitsmatrix (Identity Matrix)
mat4 mat4_identity();
// Multiplikation von zwei Matrizen
mat4 mat4_multiply(mat4 a, mat4 b);
// Erzeugt eine Translationsmatrix (Verschiebung)
mat4 mat4_translate(mat4 m, vec3 v);
// Erzeugt eine Rotationsmatrix um eine Achse (x, y oder z)
mat4 mat4_rotate(mat4 m, float angle_rad, vec3 axis);
// Erzeugt eine Skalierungsmatrix
mat4 mat4_scale(mat4 m, vec3 v);

// Erzeugt eine View-Matrix aus Kameraposition, Blickpunkt und Up-Vektor
mat4 mat4_lookAt(vec3 eye, vec3 center, vec3 up);
// Erzeugt eine perspektivische Projektionsmatrix
mat4 mat4_perspective(float fov_rad, float aspect, float near_plane, float far_plane);

// Wendet eine Matrix auf einen vec4 an
vec4 mat4_transform_vec4(mat4 m, vec4 v);

// Debug-Funktion: Gibt die Matrix auf der Konsole aus
void mat4_print(mat4 m);

#endif
