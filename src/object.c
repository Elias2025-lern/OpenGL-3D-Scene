// src/object.c

#include "object.h"
#include <stdio.h> // Für fprintf

RenderObject object_create(const float* vertices, int num_floats, int vertex_size)
{
    RenderObject new_obj;
    // Berechne die Anzahl der Vertices basierend auf der Gesamtzahl der Floats und der Floats pro Vertex
    new_obj.vertex_count = num_floats / vertex_size;
    new_obj.model_matrix = mat4_identity(); // Initialisiere die Modellmatrix

    glGenVertexArrays(1, &new_obj.VAO);
    glGenBuffers(1, &new_obj.VBO);

    if (new_obj.VAO == 0 || new_obj.VBO == 0)
    {
        fprintf(stderr, "Fehler: Konnte VAO oder VBO für das Objekt nicht generieren!\n");
        // Stelle sicher, dass die Werte initialisiert sind, damit object_destroy keine Fehler verursacht
        new_obj.VAO = 0;
        new_obj.VBO = 0;
        new_obj.vertex_count = 0;
        return new_obj; // Rückgabe eines fehlerhaften Objekts
    }

    glBindVertexArray(new_obj.VAO);

    glBindBuffer(GL_ARRAY_BUFFER, new_obj.VBO);
    glBufferData(GL_ARRAY_BUFFER, num_floats * sizeof(float), vertices, GL_STATIC_DRAW);

    // layout (location = 0) im Shader ist für Position (aPos)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertex_size * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // layout (location = 1) im Shader ist jetzt für Textur-Koordinaten (aTexCoord)
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, vertex_size * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0); // VAO lösen (unbind)
    return new_obj;
}

void object_draw(RenderObject* obj, const Shader* shader_program, const mat4* view, const mat4* projection)
{
    // Nur zeichnen, wenn VAO gültig ist
    if (obj->VAO == 0)
        return;

    // Sicherstellen, dass der Shader verwendet wird, bevor diese Funktion aufgerufen wird
    shader_use(shader_program);

    // Sende die Uniform-Matrizen
    GLint modelLoc = glGetUniformLocation(shader_program->id, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (const GLfloat*)obj->model_matrix.m);

    GLint viewLoc = glGetUniformLocation(shader_program->id, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, (const GLfloat*)view->m);

    GLint projLoc = glGetUniformLocation(shader_program->id, "projection");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, (const GLfloat*)projection->m);

    glBindVertexArray(obj->VAO);
    glDrawArrays(GL_TRIANGLES, 0, obj->vertex_count); // Zeichne das Objekt
    glBindVertexArray(0);
}

void object_destroy(RenderObject* obj)
{
    if (obj->VAO != 0)
    { // Nur löschen, wenn VAO gültig ist
        glDeleteVertexArrays(1, &obj->VAO);
        obj->VAO = 0; // Setze auf 0, um doppeltes Löschen zu vermeiden
    }
    if (obj->VBO != 0)
    { // Nur löschen, wenn VBO gültig ist
        glDeleteBuffers(1, &obj->VBO);
        obj->VBO = 0; // Setze auf 0, um doppeltes Löschen zu vermeiden
    }
}
