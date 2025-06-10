#include "models.h"
#define _USE_MATH_DEFINES // Diese Zeile muss vor dem Einbinden von math.h stehen
#include <math.h>         // Für M_PI, sin, cos
#include <stdlib.h>       // Für malloc, free
#include <stdio.h>

// Dreiecks-Daten
float triangle_vertices[] = {
    // Position          // Farbe (Rot, Grün, Blau)
    -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // Unten links (Rot)
    0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,  // Unten rechts (Grün)
    0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f    // Oben Mitte (Blau)
};
int num_triangle_triangle_floats = sizeof(triangle_vertices) / sizeof(float);
int triangle_vertex_size = 6; // 3 für Position, 3 für Farbe

// Funktion zur Erstellung der Scheibendaten für eine Kugel
// segments: Anzahl der horizontalen Segmente (von Pol zu Pol)
// rings: Anzahl der vertikalen "Ringe"
// out_num_floats: Zeiger zur Rückgabe der Gesamtzahl der erzeugten float-Werte
// out_vertex_size: Zeiger zur Rückgabe der Anzahl der float-Werte pro Vertex (3 pos + 3 Farbe = 6)
float *create_sphere_data(int segments, int rings, int *out_num_floats, int *out_vertex_size)
{
    // Jeder Vertex hat 6 floats (posX, posY, posZ, colorR, colorG, colorB)
    *out_vertex_size = 6;

    // Anzahl der Dreiecke = (segments * 2) * rings (jeder Ring hat 2*segments Dreiecke)
    // Oder genauer: 2 * segments (für die zwei Pole) + segments * (rings - 1) * 2 (für den Mittelteil)
    // Gesamtanzahl der Vertices wäre (segments * rings + 2) * 6 ohne Duplikate
    // Da wir aber Duplikate für unterschiedliche Normalen/UVs brauchen und mit GL_TRIANGLES zeichnen,
    // berechnen wir die Anzahl der Vertices für jedes Dreieck einzeln.
    // Jedes Dreieck hat 3 Vertices.
    // Dreiecke an den Polen: 2 * segments
    // Dreiecke im Mittelteil: segments * (rings - 1) * 2 (jeder "Quadrat" besteht aus 2 Dreiecken)
    int total_triangles = (2 * segments) + (segments * (rings - 1) * 2);
    int total_vertices = total_triangles * 3; // 3 Vertices pro Dreieck
    *out_num_floats = total_vertices * (*out_vertex_size);

    float *sphere_data = (float *)malloc(*out_num_floats * sizeof(float));
    if (!sphere_data)
    {
        fprintf(stderr, "Fehler: Speicher konnte nicht für Kugeldaten zugewiesen werden!\n");
        *out_num_floats = 0;
        return NULL;
    }

    int current_float_index = 0;

    // Farbe für die Kugel (z.B. Gelb)
    float r_color = 1.0f;
    float g_color = 1.0f;
    float b_color = 0.0f;

    // Erstelle die Pole
    for (int i = 0; i < segments; ++i)
    {
        // Oberstes Dreieck (Top Cap)
        // Mittelpunkt oben (Nordpol)
        sphere_data[current_float_index++] = 0.0f;
        sphere_data[current_float_index++] = 1.0f;
        sphere_data[current_float_index++] = 0.0f;
        sphere_data[current_float_index++] = r_color;
        sphere_data[current_float_index++] = g_color;
        sphere_data[current_float_index++] = b_color;

        // Punkt am ersten Ring (Top Ring)
        float theta1 = (float)i / segments * 2.0f * M_PI;
        float x1 = cos(theta1) * sin(M_PI / rings);
        float y1 = cos(M_PI / rings);
        float z1 = sin(theta1) * sin(M_PI / rings);
        sphere_data[current_float_index++] = x1;
        sphere_data[current_float_index++] = y1;
        sphere_data[current_float_index++] = z1;
        sphere_data[current_float_index++] = r_color;
        sphere_data[current_float_index++] = g_color;
        sphere_data[current_float_index++] = b_color;

        float theta2 = (float)(i + 1) / segments * 2.0f * M_PI;
        float x2 = cos(theta2) * sin(M_PI / rings);
        float y2 = cos(M_PI / rings);
        float z2 = sin(theta2) * sin(M_PI / rings);
        sphere_data[current_float_index++] = x2;
        sphere_data[current_float_index++] = y2;
        sphere_data[current_float_index++] = z2;
        sphere_data[current_float_index++] = r_color;
        sphere_data[current_float_index++] = g_color;
        sphere_data[current_float_index++] = b_color;
    }

    // Erstelle die Mittelstreifen
    for (int j = 0; j < rings - 1; ++j)
    {
        float phi1 = (float)j / rings * M_PI;
        float phi2 = (float)(j + 1) / rings * M_PI;

        for (int i = 0; i < segments; ++i)
        {
            float theta1 = (float)i / segments * 2.0f * M_PI;
            float theta2 = (float)(i + 1) / segments * 2.0f * M_PI;

            // 4 Punkte bilden 2 Dreiecke eines "Quadrats"
            float x_top_left = cos(theta1) * sin(phi1);
            float y_top_left = cos(phi1);
            float z_top_left = sin(theta1) * sin(phi1);

            float x_top_right = cos(theta2) * sin(phi1);
            float y_top_right = cos(phi1);
            float z_top_right = sin(theta2) * sin(phi1);

            float x_bottom_left = cos(theta1) * sin(phi2);
            float y_bottom_left = cos(phi2);
            float z_bottom_left = sin(theta1) * sin(phi2);

            float x_bottom_right = cos(theta2) * sin(phi2);
            float y_bottom_right = cos(phi2);
            float z_bottom_right = sin(theta2) * sin(phi2);

            // Dreieck 1
            sphere_data[current_float_index++] = x_top_left;
            sphere_data[current_float_index++] = y_top_left;
            sphere_data[current_float_index++] = z_top_left;
            sphere_data[current_float_index++] = r_color;
            sphere_data[current_float_index++] = g_color;
            sphere_data[current_float_index++] = b_color;

            sphere_data[current_float_index++] = x_bottom_left;
            sphere_data[current_float_index++] = y_bottom_left;
            sphere_data[current_float_index++] = z_bottom_left;
            sphere_data[current_float_index++] = r_color;
            sphere_data[current_float_index++] = g_color;
            sphere_data[current_float_index++] = b_color;

            sphere_data[current_float_index++] = x_top_right;
            sphere_data[current_float_index++] = y_top_right;
            sphere_data[current_float_index++] = z_top_right;
            sphere_data[current_float_index++] = r_color;
            sphere_data[current_float_index++] = g_color;
            sphere_data[current_float_index++] = b_color;

            // Dreieck 2
            sphere_data[current_float_index++] = x_top_right;
            sphere_data[current_float_index++] = y_top_right;
            sphere_data[current_float_index++] = z_top_right;
            sphere_data[current_float_index++] = r_color;
            sphere_data[current_float_index++] = g_color;
            sphere_data[current_float_index++] = b_color;

            sphere_data[current_float_index++] = x_bottom_left;
            sphere_data[current_float_index++] = y_bottom_left;
            sphere_data[current_float_index++] = z_bottom_left;
            sphere_data[current_float_index++] = r_color;
            sphere_data[current_float_index++] = g_color;
            sphere_data[current_float_index++] = b_color;

            sphere_data[current_float_index++] = x_bottom_right;
            sphere_data[current_float_index++] = y_bottom_right;
            sphere_data[current_float_index++] = z_bottom_right;
            sphere_data[current_float_index++] = r_color;
            sphere_data[current_float_index++] = g_color;
            sphere_data[current_float_index++] = b_color;
        }
    }

    // Unterstes Dreieck (Bottom Cap)
    for (int i = 0; i < segments; ++i)
    {
        // Mittelpunkt unten (Südpol)
        sphere_data[current_float_index++] = 0.0f;
        sphere_data[current_float_index++] = -1.0f;
        sphere_data[current_float_index++] = 0.0f;
        sphere_data[current_float_index++] = r_color;
        sphere_data[current_float_index++] = g_color;
        sphere_data[current_float_index++] = b_color;

        // Punkt am letzten Ring (Bottom Ring)
        float theta1 = (float)(i + 1) / segments * 2.0f * M_PI;
        float x1 = cos(theta1) * sin(M_PI - (M_PI / rings));
        float y1 = cos(M_PI - (M_PI / rings));
        float z1 = sin(theta1) * sin(M_PI - (M_PI / rings));
        sphere_data[current_float_index++] = x1;
        sphere_data[current_float_index++] = y1;
        sphere_data[current_float_index++] = z1;
        sphere_data[current_float_index++] = r_color;
        sphere_data[current_float_index++] = g_color;
        sphere_data[current_float_index++] = b_color;

        float theta2 = (float)i / segments * 2.0f * M_PI;
        float x2 = cos(theta2) * sin(M_PI - (M_PI / rings));
        float y2 = cos(M_PI - (M_PI / rings));
        float z2 = sin(theta2) * sin(M_PI - (M_PI / rings));
        sphere_data[current_float_index++] = x2;
        sphere_data[current_float_index++] = y2;
        sphere_data[current_float_index++] = z2;
        sphere_data[current_float_index++] = r_color;
        sphere_data[current_float_index++] = g_color;
        sphere_data[current_float_index++] = b_color;
    }

    // Sicherstellen, dass die Anzahl der geschriebenen floats mit der berechneten übereinstimmt
    if (current_float_index != *out_num_floats)
    {
        fprintf(stderr, "Fehler: Die Anzahl der generierten Gleitkommazahlen stimmt nicht überein!\n");
        free(sphere_data);
        *out_num_floats = 0;
        return NULL;
    }

    return sphere_data;
}

void free_sphere_data(float *sphere_data)
{
    if (sphere_data)
    {
        free(sphere_data);
    }
}
