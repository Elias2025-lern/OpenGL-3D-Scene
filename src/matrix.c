#include "matrix.h"
#include <stdio.h>  // Für printf
#include <math.h>   // Für sqrtf, sinf, cosf, tanf
#include <string.h> // Für memcpy

// --- Implementierung der Vektorfunktionen ---

vec3 vec3_create(float x, float y, float z)
{
    vec3 v = {x, y, z};
    return v;
}

vec4 vec4_create(float x, float y, float z, float w)
{
    vec4 v = {x, y, z, w};
    return v;
}

vec3 vec3_add(vec3 a, vec3 b)
{
    vec3 v = {a.x + b.x, a.y + b.y, a.z + b.z};
    return v;
}

vec3 vec3_sub(vec3 a, vec3 b)
{
    vec3 v = {a.x - b.x, a.y - b.y, a.z - b.z};
    return v;
}

// Skalaprodukt
vec3 vec3_scale(vec3 v, float s)
{
    vec3 scaled_v = {v.x * s, v.y * s, v.z * s};
    return scaled_v;
}

float vec3_dot(vec3 a, vec3 b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

vec3 vec3_cross(vec3 a, vec3 b)
{
    vec3 v = {
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x};
    return v;
}

float vec3_length(vec3 v)
{
    return sqrtf(vec3_dot(v, v));
}

vec3 vec3_normalize(vec3 v)
{
    float len = vec3_length(v);
    if (len > 0.00001f)
    { // Vermeide Division durch 0 oder sehr kleine Zahlen
        return vec3_scale(v, 1.0f / len);
    }
    return vec3_create(0.0f, 0.0f, 0.0f); // Gibt den Nullvektor zurück, wenn die Länge 0 ist
}

vec4 vec3_to_vec4(vec3 v, float w)
{
    vec4 new_v = {v.x, v.y, v.z, w};
    return new_v;
}

vec3 vec4_to_vec3(vec4 v)
{
    vec3 new_v = {v.x, v.y, v.z};
    return new_v;
}

// --- Implementierung der Matrizenfunktionen ---

mat4 mat4_identity()
{
    mat4 m;
    memset(m.m, 0, sizeof(m.m)); // Setzt alle Elemente auf 0
    m.m[0][0] = 1.0f;
    m.m[1][1] = 1.0f;
    m.m[2][2] = 1.0f;
    m.m[3][3] = 1.0f;
    return m;
}

mat4 mat4_multiply(mat4 a, mat4 b)
{
    mat4 result = mat4_identity();
    for (int i = 0; i < 4; ++i)
    { // Spalten der Ergebnis-Matrix
        for (int j = 0; j < 4; ++j)
        { // Zeilen der Ergebnis-Matrix
            result.m[i][j] = 0.0f;
            for (int k = 0; k < 4; ++k)
            {
                // Achtung: a.m[i][k] und b.m[k][j], da spaltenmajor gespeichert
                result.m[i][j] += a.m[i][k] * b.m[k][j];
            }
        }
    }
    return result;
}

mat4 mat4_translate(mat4 m, vec3 v)
{
    mat4 translation_matrix = mat4_identity();
    translation_matrix.m[3][0] = v.x; // Position x (Spalte 3, Zeile 0)
    translation_matrix.m[3][1] = v.y; // Position y (Spalte 3, Zeile 1)
    translation_matrix.m[3][2] = v.z; // Position z (Spalte 3, Zeile 2)
    return mat4_multiply(m, translation_matrix);
}

mat4 mat4_rotate(mat4 m, float angle_rad, vec3 axis)
{
    mat4 rotation_matrix = mat4_identity();
    float c = cosf(angle_rad);
    float s = sinf(angle_rad);
    float omc = 1.0f - c; // Eins minus Kosinus

    // Achse normalisieren
    axis = vec3_normalize(axis);
    float x = axis.x;
    float y = axis.y;
    float z = axis.z;

    rotation_matrix.m[0][0] = x * x * omc + c;
    rotation_matrix.m[0][1] = x * y * omc + z * s;
    rotation_matrix.m[0][2] = x * z * omc - y * s;

    rotation_matrix.m[1][0] = y * x * omc - z * s;
    rotation_matrix.m[1][1] = y * y * omc + c;
    rotation_matrix.m[1][2] = y * z * omc + x * s;

    rotation_matrix.m[2][0] = z * x * omc + y * s;
    rotation_matrix.m[2][1] = z * y * omc - x * s;
    rotation_matrix.m[2][2] = z * z * omc + c;

    return mat4_multiply(m, rotation_matrix);
}

mat4 mat4_scale(mat4 m, vec3 v)
{
    mat4 scale_matrix = mat4_identity();
    scale_matrix.m[0][0] = v.x;
    scale_matrix.m[1][1] = v.y;
    scale_matrix.m[2][2] = v.z;
    return mat4_multiply(m, scale_matrix);
}

mat4 mat4_lookAt(vec3 eye, vec3 center, vec3 up)
{
    vec3 f = vec3_normalize(vec3_sub(center, eye)); // Vorwärtsvektor
    vec3 s = vec3_normalize(vec3_cross(f, up));     // Seitlicher Vektor
    vec3 u = vec3_cross(s, f);                      // Aufwärtsvektor (neu berechnet um orthogonal zu sein)

    mat4 result = mat4_identity();
    result.m[0][0] = s.x;
    result.m[1][0] = s.y;
    result.m[2][0] = s.z;
    result.m[0][1] = u.x;
    result.m[1][1] = u.y;
    result.m[2][1] = u.z;
    result.m[0][2] = -f.x;
    result.m[1][2] = -f.y;
    result.m[2][2] = -f.z;

    result.m[3][0] = -vec3_dot(s, eye);
    result.m[3][1] = -vec3_dot(u, eye);
    result.m[3][2] = vec3_dot(f, eye);
    // f ist der Vektor von eye zu center, deshalb ist das Skalarprodukt mit eye die negative Distanz von Ursprung bis zur Ebene orthogonal zu f und durch eye

    return result;
}

mat4 mat4_perspective(float fov_rad, float aspect, float near_plane, float far_plane)
{
    mat4 result = mat4_identity();
    float tan_half_fov = tanf(fov_rad / 2.0f);

    result.m[0][0] = 1.0f / (aspect * tan_half_fov);
    result.m[1][1] = 1.0f / tan_half_fov;
    result.m[2][2] = -(far_plane + near_plane) / (far_plane - near_plane);
    result.m[2][3] = -1.0f; // Für W = -Z (ermöglicht Division durch w)
    result.m[3][2] = -(2.0f * far_plane * near_plane) / (far_plane - near_plane);
    result.m[3][3] = 0.0f; // Sicherstellen, dass W nicht vom Z-Wert der Ecke beeinflusst wird, sondern vom -Z der Projektion

    return result;
}

vec4 mat4_transform_vec4(mat4 m, vec4 v)
{
    vec4 result;
    result.x = m.m[0][0] * v.x + m.m[1][0] * v.y + m.m[2][0] * v.z + m.m[3][0] * v.w;
    result.y = m.m[0][1] * v.x + m.m[1][1] * v.y + m.m[2][1] * v.z + m.m[3][1] * v.w;
    result.z = m.m[0][2] * v.x + m.m[1][2] * v.y + m.m[2][2] * v.z + m.m[3][2] * v.w;
    result.w = m.m[0][3] * v.x + m.m[1][3] * v.y + m.m[2][3] * v.z + m.m[3][3] * v.w;
    return result;
}

void mat4_print(mat4 m)
{
    printf("Matrix:\n");
    for (int j = 0; j < 4; ++j)
    { // Zeilen
        for (int i = 0; i < 4; ++i)
        { // Spalten
            printf("%8.3f ", m.m[i][j]);
        }
        printf("\n");
    }
}
