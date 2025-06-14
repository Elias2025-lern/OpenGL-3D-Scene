#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>        // Für printf, fprintf
#include <stdlib.h>       // Für exit()
#include <stdbool.h>      // Für bool
#define _USE_MATH_DEFINES // Muss vor dem Einbinden von math.h stehen
#include <math.h>
#include "utils.h"  // Enthält Hilfsfunktionen
#include "shader.h" // Enthält Shader-Modul
#include "matrix.h"
#include "models.h"
#include "object.h"

const char *WINDOW_TITLE = "OpenGL";

// Unser Shader-Objekt
Shader basicColorShader;

RenderObject triangle_obj; // Objekt, das das Dreieck darstellt
RenderObject sphere_obj;   // Objekt, das die Kugel darstellt

// Variablen zur Speicherung der dynamisch erzeugten Kugeldaten (wichtig für spätere Freigabe)
float *sphere_data_ptr = NULL;
int sphere_num_floats = 0;
int sphere_vertex_size = 0;

// Callback-Funktion, die aufgerufen wird, wenn die Fenstergröße geändert wird
void framebuffer_size_callback(GLFWwindow *window, int width, int height);

// --- Hauptprogramm ---
int main()
{
    GLFWwindow *window;

    // Schritt 1: Initialisiere GLFW
    // Kein Fehler-Callback für GLFW registrieren, wie gewünscht.
    if (!glfwInit())
    {
        fprintf(stderr, "GLFW konnte nicht initialisiert werden\n");
        return EXIT_FAILURE;
    }

    // Schritt 2: Konfiguriere GLFW und OpenGL-Kontext
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // Fenster soll vergrößerbar sein (Standard, aber explizit setzen)
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

    // Schritt 3: Erstelle GLFW-Fenster - FENSTERGRÖßE 800x600
    window = glfwCreateWindow(800, 600, WINDOW_TITLE, NULL, NULL);
    if (!window)
    {
        fprintf(stderr, "Konnte GLFW-Fenster nicht erstellen\n");
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Schritt 4: Initialisiere GLEW
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        fprintf(stderr, "GLEW konnte nicht initialisiert werden: %s\n", glewGetErrorString(err));
        glfwDestroyWindow(window); // Fenster zerstören
        glfwTerminate();           // GLFW freigeben
        return EXIT_FAILURE;
    }

    // OpenGL- und GLSL-Version ausgeben (zur Kontrolle)
    printf("OpenGL Version: %s\n", glGetString(GL_VERSION));
    printf("GLSL Version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

    // Schritt 6: Shader-Programm aus Dateien laden
    basicColorShader = shader_create("shaders/basic_color.vert", "shaders/basic_color.frag");
    if (basicColorShader.id == 0)
    {
        fprintf(stderr, "Shader-Programm konnte nicht erstellt werden!\n");
        glfwDestroyWindow(window);
        glfwTerminate();
        return EXIT_FAILURE;
    }

    // 1. DREIECK ERSTELLEN
    // object_create() aufrufen, um VAO/VBO für das Dreieck einzurichten
    // Die Daten triangle_vertices sind in src/models.c definiert und in include/models.h deklariert
    triangle_obj = object_create(triangle_vertices, num_triangle_triangle_floats, triangle_vertex_size);
    if (triangle_obj.VAO == 0)
    { // Prüfen, ob das Objekt erfolgreich erstellt wurde
        fprintf(stderr, "Konnte Dreieck-Objekt nicht erstellen!\n");
        // Bereits erstellte Ressourcen freigeben
        shader_destroy(&basicColorShader);
        glfwDestroyWindow(window);
        glfwTerminate();
        return EXIT_FAILURE;
    }

    // 2. KUGEL ERSTELLEN
    // Zuerst die Vertex-Daten für die Kugel generieren mit create_sphere_data()
    // Parameter: Segmente (horizontale Auflösung), Ringe (vertikale Auflösung), sowie Zeiger zur Größenrückgabe
    sphere_data_ptr = create_sphere_data(32, 16, &sphere_num_floats, &sphere_vertex_size);
    if (sphere_data_ptr == NULL || sphere_num_floats == 0)
    {
        fprintf(stderr, "Konnte Kugeldaten nicht generieren!\n");
        object_destroy(&triangle_obj); // Dreieck aufräumen, falls vorhanden
        shader_destroy(&basicColorShader);
        glfwDestroyWindow(window);
        glfwTerminate();
        return EXIT_FAILURE;
    }

    // Dann RenderObject aus den generierten Kugeldaten erstellen
    sphere_obj = object_create(sphere_data_ptr, sphere_num_floats, sphere_vertex_size);
    if (sphere_obj.VAO == 0)
    {
        fprintf(stderr, "Konnte Kugel-Objekt nicht erstellen!\n");
        free_sphere_data(sphere_data_ptr); // SEHR WICHTIG: Speicher der Kugeldaten freigeben
        object_destroy(&triangle_obj);
        shader_destroy(&basicColorShader);
        glfwDestroyWindow(window);
        glfwTerminate();
        return EXIT_FAILURE;
    }


    // Schritt 8: Haupt-Render-Schleife
    // Schleife läuft, bis das Fenster geschlossen wird
    while (!glfwWindowShouldClose(window))
    {
        // Benutzereingaben verarbeiten (z.B. ESC-Taste), definiert in utils.c
        processInput(window);

        // Eigene Modelltransformation auf das Dreieck anwenden (verschieben und rotieren)
        float angle = (float)glfwGetTime();
        triangle_obj.model_matrix = mat4_identity();
        triangle_obj.model_matrix = mat4_rotate(triangle_obj.model_matrix, angle, vec3_create(0.0f, 1.0f, 0.0f));
        triangle_obj.model_matrix = mat4_translate(triangle_obj.model_matrix, vec3_create(-2.75f, 0.0f, 0.0f));
        
        // Eigene Modelltransformation auf die Kugel anwenden (verschieben und skalieren)
        sphere_obj.model_matrix = mat4_identity();
        sphere_obj.model_matrix = mat4_scale(sphere_obj.model_matrix, vec3_create(1.5f, 1.5f, 1.5f)); // Kugel verkleinern
        sphere_obj.model_matrix = mat4_rotate(sphere_obj.model_matrix, angle, vec3_create(1.0f, 0.0f, 0.0f));

        // Kamera befindet sich bei (0,0,7), blickt auf Center(0,0,0), "up"-Vektor ist (0,1,0)
        mat4 view_matrix = mat4_lookAt(vec3_create(0.0f, 0.0f, 7.0f),
                                       vec3_create(0.0f, 0.0f, 0.0f),
                                       vec3_create(0.0f, 1.0f, 0.0f));

        // Perspektivprojektion einrichten (Projection-Matrix)
        // Blickwinkel (FOV): 45 Grad (in Radiant umgerechnet)
        // Seitenverhältnis: abhängig von Fenstergröße
        // Near Plane: 0.1f, Far Plane: 100.0f
        int window_width, window_height;
        glfwGetWindowSize(window, &window_width, &window_height);
        float aspect_ratio = (float)window_width / (float)window_height;
        mat4 projection_matrix = mat4_perspective(45.0f * (M_PI / 180.0f), aspect_ratio, 0.1f, 100.0f);

        // --- Rendering-Befehle ---
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // Hintergrundfarbe setzen (dunkles Petrol)
        glClear(GL_COLOR_BUFFER_BIT);         // Nur den Farb-Buffer löschen (kein Tiefentest nötig für 2D-Dreieck)
        
        // Anstatt glBindVertexArray() und glDrawArrays() direkt aufzurufen,
        // verwenden wir object_draw() für jedes Objekt.
        // Diese Funktion kümmert sich selbst um das Senden der model_matrix,
        // das Binden des VAO und das Ausführen des Zeichnungsbefehls.
        object_draw(&triangle_obj, &basicColorShader, &view_matrix, &projection_matrix);

        object_draw(&sphere_obj, &basicColorShader, &view_matrix, &projection_matrix); // Kugel zeichnen

        glfwSwapBuffers(window); // Front- und Backbuffer tauschen, um Frame anzuzeigen
        glfwPollEvents();        // Ereignisse verarbeiten (Tastatur, Maus, Fenstergröße)
    }

    // Schritt 9: Ressourcen freigeben
    fprintf(stdout, "Fenster schließen und aufräumen...\n");

    // --- LETZTE ÄNDERUNG: Ressourcen der einzelnen Objekte freigeben ---
    // object_destroy() für jedes erstellte RenderObject aufrufen
    object_destroy(&triangle_obj);
    object_destroy(&sphere_obj);

    // WICHTIG: Speicher der dynamisch allozierten Kugeldaten freigeben
    free_sphere_data(sphere_data_ptr);

    // ... (Shader und GLFW freigeben - KEINE ÄNDERUNGEN) ...
    shader_destroy(&basicColorShader);
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // Sicherstellen, dass der Viewport mit der neuen Fenstergröße übereinstimmt
    glViewport(0, 0, width, height);
}
