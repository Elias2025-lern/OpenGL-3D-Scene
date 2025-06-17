#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "obj_loader.h"

// Funktion zum Laden einer OBJ-Datei
// Parameter:
// - filename: Pfad zur OBJ-Datei
// - out_vertices: Pointer zum Speichern der Vertex-Daten
// - out_num_floats: Pointer zum Speichern der Anzahl der Floats
// Rückgabe: true bei Erfolg, false bei Fehler
bool load_obj(const char* filename, float** out_vertices, int* out_num_floats) {
    // Datei öffnen
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Konnte Datei %s nicht öffnen\n", filename);
        return false;
    }

    // Temporäre Arrays für die Verarbeitung
    float temp_vertices[100000];  // Puffer für die endgültigen Vertex-Daten
    int vertex_count = 0;         // Zähler für Vertex-Daten

    // Arrays für geparste Daten aus der OBJ-Datei
    float parsed_vertices[10000][3];   // 3D-Positionen (x,y,z)
    float parsed_texcoords[10000][2];  // Texturkoordinaten (u,v)
    int parsed_vertex_count = 0;       // Zähler für Positionen
    int parsed_texcoord_count = 0;     // Zähler für Texturkoordinaten

    // Datei zeilenweise lesen
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        // Vertex-Position (v x y z)
        if (strncmp(line, "v ", 2) == 0) {
            float x, y, z;
            sscanf(line, "v %f %f %f", &x, &y, &z);
            parsed_vertices[parsed_vertex_count][0] = x;
            parsed_vertices[parsed_vertex_count][1] = y;
            parsed_vertices[parsed_vertex_count][2] = z;
            parsed_vertex_count++;
        }
        // Texturkoordinaten (vt u v)
        else if (strncmp(line, "vt ", 3) == 0) {
            float u, v;
            sscanf(line, "vt %f %f", &u, &v);
            parsed_texcoords[parsed_texcoord_count][0] = u;
            parsed_texcoords[parsed_texcoord_count][1] = v;
            parsed_texcoord_count++;
        }
        // Flächendefinition (f v1/vt1 v2/vt2 v3/vt3)
        else if (strncmp(line, "f ", 2) == 0) {
            int v1, vt1, v2, vt2, v3, vt3;
            // OBJ-Format: f vertex_index/texture_index ...
            if (sscanf(line, "f %d/%d %d/%d %d/%d", &v1, &vt1, &v2, &vt2, &v3, &vt3) == 6) {
                // Vertex-Daten für die drei Eckpunkte des Dreiecks holen
                float* a = parsed_vertices[v1 - 1];  // OBJ-Indizes starten bei 1
                float* ta = parsed_texcoords[vt1 - 1];
                float* b = parsed_vertices[v2 - 1];
                float* tb = parsed_texcoords[vt2 - 1];
                float* c = parsed_vertices[v3 - 1];
                float* tc = parsed_texcoords[vt3 - 1];

                // Daten in temp_vertices speichern (Position + Texturkoordinaten)
                // Format: x,y,z, u,v

                // Erster Eckpunkt
                temp_vertices[vertex_count++] = a[0];  // x
                temp_vertices[vertex_count++] = a[1];  // y
                temp_vertices[vertex_count++] = a[2];  // z
                temp_vertices[vertex_count++] = ta[0]; // u
                temp_vertices[vertex_count++] = ta[1]; // v

                // Zweiter Eckpunkt
                temp_vertices[vertex_count++] = b[0];
                temp_vertices[vertex_count++] = b[1];
                temp_vertices[vertex_count++] = b[2];
                temp_vertices[vertex_count++] = tb[0];
                temp_vertices[vertex_count++] = tb[1];

                // Dritter Eckpunkt
                temp_vertices[vertex_count++] = c[0];
                temp_vertices[vertex_count++] = c[1];
                temp_vertices[vertex_count++] = c[2];
                temp_vertices[vertex_count++] = tc[0];
                temp_vertices[vertex_count++] = tc[1];
            }
        }
    }

    // Datei schließen
    fclose(file);

    // Speicher für die Vertex-Daten allozieren
    *out_vertices = malloc(vertex_count * sizeof(float));
    // Daten aus dem temporären Array kopieren
    memcpy(*out_vertices, temp_vertices, vertex_count * sizeof(float));
    // Anzahl der Floats zurückgeben
    *out_num_floats = vertex_count;

    return true;
}