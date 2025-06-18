#ifndef MODELS_H
#define MODELS_H

// Daten für das Dreieck
extern float triangle_vertices[];
extern int num_triangle_triangle_floats; // Gesamtanzahl der Floats für das Dreieck
extern int triangle_vertex_size;         // Anzahl der Floats pro Vertex des Dreiecks (z. B.: 6)

// Funktionsdeklaration zum Erzeugen der Vertexdaten für eine Kugel
// Gibt einen Zeiger auf ein Float-Array zurück und aktualisiert die Anzahl der Floats sowie die Vertexgröße
float *create_sphere_data(int segments, int rings, int *out_num_floats, int *out_vertex_size);
// Funktion zum Freigeben des dynamisch allokierten Speichers für die Kugeldaten
void free_sphere_data(float *sphere_data);

#endif // MODELS_H
