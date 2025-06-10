#ifndef OBJECT_H
#define OBJECT_H

#include <GL/glew.h>
#include "matrix.h"
#include "shader.h"

// --- Datenstruktur für ein Objekt (RenderObject) ---
// Dies ist das "Profil" oder die "Vorlage" für jedes Objekt, das du in der Szene zeichnen möchtest.
// Es enthält alle notwendigen Informationen, damit OpenGL weiß, wie dieses Objekt gezeichnet werden soll.
typedef struct
{
    GLuint VAO; // Vertex Array Object: „Speichert“ die Konfiguration zum Lesen der Vertexdaten
    GLuint VBO; // Vertex Buffer Object: Enthält die tatsächlichen Vertexdaten (Position, Farbe usw.)
    // GLuint EBO;         // Element Buffer Object: Falls Indizes verwendet werden, um Duplikate zu vermeiden

    int vertex_count; // Anzahl der zu zeichnenden Vertices für dieses Objekt (z. B. 3 für ein Dreieck
                      // oder die Gesamtanzahl der Vertices für eine Kugel nach der Erzeugung)

    mat4 model_matrix; // Modellmatrix: Bestimmt die Position, Ausrichtung und Größe
                       // dieses Objekts im Weltkoordinatensystem.

    // Du kannst hier weitere Eigenschaften je nach Bedarf hinzufügen:
    // Shader* object_shader; // Zeiger auf einen eigenen Shader für das Objekt (falls verwendet)
    // Material object_material; // Informationen zum Material (Oberflächenfarbe, Glanz usw.)
    // Texture object_texture;   // Informationen zur Textur (Oberflächenbild)
} RenderObject;

// --- Funktionsdeklarationen zur Verwaltung von Objekten ---

// Funktion zur Erstellung eines neuen RenderObject.
// Parameter:
//   - vertices: Zeiger auf das Array mit Vertexdaten (z. B. triangle_vertices oder Kugeldaten)
//   - num_floats: Gesamtanzahl der float-Werte im Vertexdaten-Array
//   - vertex_size: Anzahl der float-Werte pro VERTEX (z. B. 6 bei 3 Positionen + 3 Farben)
// Gibt eine initialisierte RenderObject-Struktur mit eingerichteten VAO/VBO zurück.
RenderObject object_create(const float *vertices, int num_floats, int vertex_size);

// Funktion zum Zeichnen eines RenderObject auf dem Bildschirm.
// Parameter:
//   - obj: Zeiger auf das zu zeichnende RenderObject
//   - shader_program: Zeiger auf das Shader-Programm (häufig für die gesamte Szene oder Objektgruppe)
//   - view: Zeiger auf die View-Matrix der Szene (von der Kamera)
//   - projection: Zeiger auf die Projektionsmatrix der Szene (Perspektive)
void object_draw(RenderObject *obj, const Shader *shader_program, const mat4 *view, const mat4 *projection);

// Funktion zum Freigeben der OpenGL-Ressourcen eines RenderObject.
// (Löscht VAO und VBO vom GPU-Speicher)
// Parameter:
//   - obj: Zeiger auf das RenderObject, das gelöscht werden soll
void object_destroy(RenderObject *obj);

#endif // OBJECTS_H
