#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>   // Für printf, fprintf
#include <stdlib.h>  // Für exit()
#include <stdbool.h> // Für bool

#include "utils.h"  // Enthält Hilfsfunktionen
#include "shader.h" // Enthält Shader-Modul
#include "matrix.h"

const char *WINDOW_TITLE = "OpenGL";

// --- Definiere Daten für ein Dreieck mit pro-Vertex Farben ---
// Jeder Vertex hat: 3 Positionswerte (X, Y, Z) und 3 Farbwerte (R, G, B)
// Layout: PosX, PosY, PosZ, FarbeR, FarbeG, FarbeB
float vertices[] = {
    // Position (X, Y, Z)  // Farbe (R, G, B)
    -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // Unten links (Rot)
    0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,  // Unten rechts (Grün)
    0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f    // Oben Mitte (Blau)
};

// VAO und VBO für unser Dreieck
GLuint VAO, VBO;

// Unser Shader-Objekt
Shader basicColorShader;

// --- Schritt 5: Funktion zum Einrichten von OpenGL-Puffern (VAO, VBO) ---
bool setup_buffers()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Konfiguriere Vertex-Attribut (Position):
    // layout (location = 0) im Shader ist für Position (aPos)
    // 3 floats pro Position, nicht normalisiert
    // stride ist 6 * sizeof(float), da jeder Vertex 6 floats hat (3 pos + 3 Farbe)
    // Offset ist 0 (beginnt am Anfang der Vertex-Daten)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // Konfiguriere Vertex-Attribut (Farbe):
    // layout (location = 1) im Shader ist für Farbe (aColor)
    // 3 floats pro Farbe, nicht normalisiert
    // stride ist 6 * sizeof(float) (Gesamtgröße eines Vertex)
    // Offset ist 3 * sizeof(float) (beginnt nach den 3 Positionsfloats)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0); // VAO entbinden, um versehentliche Änderungen zu vermeiden
    return true;
}

// --- Hauptprogramm ---
int main()
{
    GLFWwindow *window;

    // Schritt 1: Initialisiere GLFW
    // Registriere keinen Fehler-Callback für GLFW, wie gewünscht.
    if (!glfwInit())
    {
        fprintf(stderr, "GLFW konnte nicht initialisiert werden\n");
        return EXIT_FAILURE;
    }

    // Schritt 2: Konfiguriere GLFW und OpenGL-Kontext
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // Für Vollbild wird GLFW_RESIZABLE nicht deaktiviert,
    // GLFW verwaltet die Fenstergröße automatisch.
    // glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Schritt 3: Erstelle GLFW-Fenster - FÜR VOLL BILDSCHIRM GEÄNDERT
    // Hole den Hauptmonitor (Primärbildschirm)
    GLFWmonitor *primaryMonitor = glfwGetPrimaryMonitor();
    if (!primaryMonitor)
    {
        fprintf(stderr, "Konnte Hauptmonitor nicht ermitteln!\n");
        glfwTerminate();
        return EXIT_FAILURE;
    }

    // Hole aktuellen Video-Modus des Hauptmonitors
    const GLFWvidmode *mode = glfwGetVideoMode(primaryMonitor);
    if (!mode)
    {
        fprintf(stderr, "Konnte Video-Modus des Hauptmonitors nicht ermitteln!\n");
        glfwTerminate();
        return EXIT_FAILURE;
    }

    // Erstelle Fenster im Vollbildmodus
    // Fenstergröße entspricht der aktuellen Bildschirmauflösung
    // Der vierte Parameter ist der Monitor, hier wird primaryMonitor für Vollbild übergeben
    window = glfwCreateWindow(mode->width, mode->height, WINDOW_TITLE, primaryMonitor, NULL);
    if (!window)
    {
        fprintf(stderr, "Konnte GLFW-Fenster nicht erstellen\n");
        glfwTerminate(); // Ressourcen freigeben
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window); // Setze den OpenGL-Kontext auf das Fenster

    // Schritt 4: Initialisiere GLEW
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        fprintf(stderr, "GLEW konnte nicht initialisiert werden: %s\n", glewGetErrorString(err));
        glfwDestroyWindow(window); // Fenster zerstören
        glfwTerminate();           // GLFW freigeben
        return EXIT_FAILURE;
    }

    // OpenGL und GLSL Version ausgeben (zur Kontrolle)
    printf("OpenGL Version: %s\n", glGetString(GL_VERSION));
    printf("GLSL Version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

    // Schritt 5: OpenGL-Puffer für das Dreieck einrichten
    if (!setup_buffers())
    {
        fprintf(stderr, "OpenGL-Puffer konnten nicht eingerichtet werden!\n");
        glfwDestroyWindow(window);
        glfwTerminate();
        return EXIT_FAILURE;
    }

    // Schritt 6: Shader-Programm aus Dateien laden
    basicColorShader = shader_create("shaders/basic_color.vert", "shaders/basic_color.frag");
    if (basicColorShader.id == 0)
    { // Überprüfe, ob Shader-Erstellung erfolgreich war
        fprintf(stderr, "Shader-Programm konnte nicht erstellt werden!\n");
        // Puffer vor Beenden aufräumen, falls Shader-Erstellung fehlschlägt
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glfwDestroyWindow(window);
        glfwTerminate();
        return EXIT_FAILURE;
    }

    // Schritt 7: Matrizen setzen (Neu)
    mat4 model_matrix = mat4_identity();      // Modellmatrix (Translation, Rotation, Skalierung des Objekts)
    mat4 view_matrix = mat4_identity();       // View-Matrix (Position der Kamera)
    mat4 projection_matrix = mat4_identity(); // Projektionsmatrix (Perspektive)

    // Beispiel für Transformationen:
    // Verschiebe das Dreieck etwas nach hinten, damit es nicht genau auf der Kamera liegt
    // (OpenGL schaut standardmäßig in Richtung -Z)
    model_matrix = mat4_translate(model_matrix, vec3_create(0.0f, 0.0f, -2.0f));

    // Drehe das Dreieck etwas um die Y-Achse
    // 45 Grad in Radiant umrechnen: 45 * (M_PI / 180.0f)
#define M_PI 3.14159265358979323846f // Definiere M_PI, falls noch nicht definiert
    model_matrix = mat4_rotate(model_matrix, 45.0f * (M_PI / 180.0f), vec3_create(0.0f, 1.0f, 0.0f));

    // Kamera einstellen (View-Matrix)
    // Kamera steht bei (0,0,3), schaut auf (0,0,0), "up"-Vektor ist (0,1,0)
    view_matrix = mat4_lookAt(vec3_create(0.0f, 0.0f, 3.0f),  // eye (Kameraposition)
                              vec3_create(0.0f, 0.0f, 0.0f),  // center (Blickpunkt)
                              vec3_create(0.0f, 1.0f, 0.0f)); // up (Aufwärtsrichtung der Kamera)

    // Perspektive einstellen (Projektionsmatrix)
    // Sichtwinkel (FOV): 45 Grad (in Radiant umgerechnet)
    // Seitenverhältnis (Aspect Ratio): abhängig von Fenstergröße
    // Near Plane: 0.1f, Far Plane: 100.0f
    float aspect_ratio = (float)mode->width / (float)mode->height; // Aus Bildschirmgröße im Vollbildmodus
    projection_matrix = mat4_perspective(45.0f * (M_PI / 180.0f), aspect_ratio, 0.1f, 100.0f);

    // Schritt 8: Haupt-Render-Schleife
    // Schleife läuft, bis das Fenster geschlossen wird
    while (!glfwWindowShouldClose(window))
    {
        // Benutzer-Eingaben verarbeiten (z.B. ESC Taste), definiert in utils.c
        processInput(window);

        // --- Render Befehle ---
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // Hintergrundfarbe setzen (dunkles Petrol)
        glClear(GL_COLOR_BUFFER_BIT);         // Nur den Farb-Puffer löschen (kein Tiefentest nötig für 2D-Dreieck)

        // Shader-Programm verwenden
        shader_use(&basicColorShader);

        /* MATRIZEN BENUTZEN */
        // Senden der Matrizen an den Shader
        // Hole die Position des Uniforms im Shader und übergebe die Matrix dorthin
        GLint modelLoc = glGetUniformLocation(basicColorShader.id, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (const GLfloat *)model_matrix.m);

        GLint viewLoc = glGetUniformLocation(basicColorShader.id, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, (const GLfloat *)view_matrix.m);

        GLint projLoc = glGetUniformLocation(basicColorShader.id, "projection");
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, (const GLfloat *)projection_matrix.m);

        // Vertex Array Object (VAO) binden, um OpenGL mitzuteilen, welche Daten verwendet werden
        glBindVertexArray(VAO);
        // Dreieck zeichnen mit 3 Vertices (Primitive Typ GL_TRIANGLES)
        glDrawArrays(GL_TRIANGLES, 0, 3);
        // VAO entbinden, um unbeabsichtigte Änderungen zu vermeiden
        glBindVertexArray(0);

        // --- Render Befehle Ende ---

        glfwSwapBuffers(window); // Tausche Vorder- und Hintergrund-Puffer, um Frame anzuzeigen
        glfwPollEvents();        // Verarbeite ausstehende Events (Tastatur, Maus, Fenstergröße)
    }

    // Schritt 9: Ressourcen freigeben
    fprintf(stdout, "Fenster schließen und aufräumen...\n");
    // OpenGL-Ressourcen freigeben
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    shader_destroy(&basicColorShader); // Shader-Programm freigeben
    // GLFW-Ressourcen freigeben
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0; // Programm erfolgreich beenden
}
