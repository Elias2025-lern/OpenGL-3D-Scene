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
#define DEG2RAD(x) ((x) * (3.14159265f / 180.0f))

const char *WINDOW_TITLE = "OpenGL";

// Unser Shader-Objekt
Shader basicColorShader;

RenderObject triangle_obj;    // Objekt, das das Dreieck darstellt
RenderObject sphere_obj;      // Objekt, das die Kugel darstellt
RenderObject blue_sphere_obj; // Objekt für die blaue Kugel

// Variablen zur Speicherung der dynamisch erzeugten Kugeldaten (wichtig für spätere Freigabe)
float *sphere_data_ptr = NULL;
int sphere_num_floats = 0;
int sphere_vertex_size = 0;

// Variablen für die blaue Kugel
float *blue_sphere_data_ptr = NULL;
int blue_sphere_num_floats = 0;
int blue_sphere_vertex_size = 0;

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

    // Tiefentest aktivieren
    glEnable(GL_DEPTH_TEST);

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
    sphere_data_ptr = create_sphere_data(1.0f, 1.0f, 0.0f, 32, 16, &sphere_num_floats, &sphere_vertex_size); // Ví dụ: màu vàng (R=1, G=1, B=0)
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

    // 3. BLAUE KUGEL ERSTELLEN (kleiner und um die gelbe Kugel kreisend)
    blue_sphere_data_ptr = create_sphere_data(0.0f, 0.0f, 1.0f, 24, 12, &blue_sphere_num_floats, &blue_sphere_vertex_size); // Blaue Farbe
    if (blue_sphere_data_ptr == NULL || blue_sphere_num_floats == 0)
    {
        fprintf(stderr, "Konnte Daten für blaue Kugel nicht generieren!\n");
        free_sphere_data(sphere_data_ptr);
        object_destroy(&triangle_obj);
        object_destroy(&sphere_obj);
        shader_destroy(&basicColorShader);
        glfwDestroyWindow(window);
        glfwTerminate();
        return EXIT_FAILURE;
    }
    blue_sphere_obj = object_create(blue_sphere_data_ptr, blue_sphere_num_floats, blue_sphere_vertex_size);
    if (blue_sphere_obj.VAO == 0)
    {
        fprintf(stderr, "Konnte blaue Kugel-Objekt nicht erstellen!\n");
        free_sphere_data(sphere_data_ptr);
        free_sphere_data(blue_sphere_data_ptr);
        object_destroy(&triangle_obj);
        object_destroy(&sphere_obj);
        shader_destroy(&basicColorShader);
        glfwDestroyWindow(window);
        glfwTerminate();
        return EXIT_FAILURE;
    }

    // Kamera initialisiert
    vec3 camera_pos = vec3_create(0.0f, 0.0f, 10.0f);
    vec3 camera_front = vec3_create(0.0f, 0.0f, -1.0f);
    vec3 camera_up = vec3_create(0.0f, 1.0f, 0.0f);

    // Schritt 8: Haupt-Render-Schleife
    // Schleife läuft, bis das Fenster geschlossen wird
    while (!glfwWindowShouldClose(window))
    {
        // Benutzereingaben verarbeiten (z.B. ESC-Taste), definiert in utils.c
        processInput(window);

        const float camera_speed = 0.05f;

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            camera_pos.z -= camera_speed;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            camera_pos.z += camera_speed;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            camera_pos.x -= camera_speed;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            camera_pos.x += camera_speed;
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
            camera_pos.y += camera_speed;
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
            camera_pos.y -= camera_speed;

        // Eigene Modelltransformation auf das Dreieck anwenden (verschieben und rotieren)
        float angle = (float)glfwGetTime();
        triangle_obj.model_matrix = mat4_identity();
        triangle_obj.model_matrix = mat4_rotate(triangle_obj.model_matrix, angle, vec3_create(0.0f, 1.0f, 0.0f));
        triangle_obj.model_matrix = mat4_translate(triangle_obj.model_matrix, vec3_create(-2.75f, 0.0f, 0.0f));

        // Eigene Modelltransformation auf die Kugel anwenden (verschieben und skalieren) a
        sphere_obj.model_matrix = mat4_identity();
        sphere_obj.model_matrix = mat4_scale(sphere_obj.model_matrix, vec3_create(1.5f, 1.5f, 1.5f)); // Kugel verkleinern
        sphere_obj.model_matrix = mat4_rotate(sphere_obj.model_matrix, angle, vec3_create(1.0f, 0.0f, 0.0f));

        // Modelltransformation für die blaue Kugel
        blue_sphere_obj.model_matrix = mat4_identity();
        // 1. Um die gelbe Kugel rotieren (Y-Achse)
        blue_sphere_obj.model_matrix = mat4_rotate(blue_sphere_obj.model_matrix, angle * 0.5f, vec3_create(0.0f, 1.0f, 0.0f));
        // 2. Auf den Orbit-Radius verschieben (entlang der X-Achse nach der Rotation)
        blue_sphere_obj.model_matrix = mat4_translate(blue_sphere_obj.model_matrix, vec3_create(5.0f, 5.0f, 0.0f));
        // 3. Die blaue Kugel skalieren (kleiner machen)
        blue_sphere_obj.model_matrix = mat4_scale(blue_sphere_obj.model_matrix, vec3_create(0.5f, 0.5f, 0.5f));
        // 4. Eigene Rotation der blauen Kugel (optional)
        blue_sphere_obj.model_matrix = mat4_rotate(blue_sphere_obj.model_matrix, angle * 2.0f, vec3_create(0.0f, 0.0f, 1.0f));

        mat4 view_matrix = mat4_lookAt(camera_pos,
                                       vec3_add(camera_pos, camera_front),
                                       camera_up);

        // Perspektivprojektion einrichten (Projection-Matrix) - unverändert
        // Blickwinkel (FOV): 45 Grad (in Radiant umgerechnet)
        // Seitenverhältnis: abhängig von Fenstergröße
        // Near Plane: 0.1f, Far Plane: 100.0f
        int window_width, window_height;
        glfwGetWindowSize(window, &window_width, &window_height);
        float aspect_ratio = (float)window_width / (float)window_height;
        mat4 projection_matrix = mat4_perspective(45.0f * (M_PI / 180.0f), aspect_ratio, 0.1f, 100.0f);

        // --- Rendering-Befehle ---
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);               // Hintergrundfarbe etwas dunkler
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Farb- und Tiefenbuffer löschen

        // Shader aktivieren (wird auch in object_draw gemacht, aber hier für globale Uniforms)
        shader_use(&basicColorShader);

        // Licht-Uniforms setzen
        // (Sie könnten shader_set_vec3 Funktionen in shader.c hinzufügen für bessere Lesbarkeit)
        vec3 light_pos_world = vec3_create(1.2f, 2.0f, 3.0f);
        glUniform3f(glGetUniformLocation(basicColorShader.id, "lightPos_world"), light_pos_world.x, light_pos_world.y, light_pos_world.z);
        glUniform3f(glGetUniformLocation(basicColorShader.id, "viewPos_world"), camera_pos.x, camera_pos.y, camera_pos.z);
        glUniform3f(glGetUniformLocation(basicColorShader.id, "lightColor"), 1.0f, 1.0f, 1.0f); // Weißes Licht

        // Anstatt glBindVertexArray() und glDrawArrays() direkt aufzurufen,
        // verwenden wir object_draw() für jedes Objekt.
        // Diese Funktion kümmert sich selbst um das Senden der model_matrix,
        // das Binden des VAO und das Ausführen des Zeichnungsbefehls.
        // Der Shader sollte bereits aktiv sein und die globalen Licht-Uniforms gesetzt.

        object_draw(&triangle_obj, &basicColorShader, &view_matrix, &projection_matrix);

        object_draw(&sphere_obj, &basicColorShader, &view_matrix, &projection_matrix);      // Kugel zeichnen
        object_draw(&blue_sphere_obj, &basicColorShader, &view_matrix, &projection_matrix); // Blaue Kugel zeichnen

        glfwSwapBuffers(window); // Front- und Backbuffer tauschen, um Frame anzuzeigen
        glfwPollEvents();        // Ereignisse verarbeiten (Tastatur, Maus, Fenstergröße)
    }

    // Schritt 9: Ressourcen freigeben
    fprintf(stdout, "Fenster schließen und aufräumen...\n");

    // --- LETZTE ÄNDERUNG: Ressourcen der einzelnen Objekte freigeben ---
    // object_destroy() für jedes erstellte RenderObject aufrufen
    object_destroy(&triangle_obj);
    object_destroy(&sphere_obj);
    object_destroy(&blue_sphere_obj);

    // WICHTIG: Speicher der dynamisch allozierten Kugeldaten freigeben
    free_sphere_data(sphere_data_ptr);
    free_sphere_data(blue_sphere_data_ptr);

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
