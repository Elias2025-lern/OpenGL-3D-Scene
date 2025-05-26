#ifndef UTILS_H
#define UTILS_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>   // Für fprintf
#include <stdbool.h> // Für bool
#include <assert.h>  // Für assert

// --- Fehlerbehandlung für GLFW ---
// Diese Funktion mit glfwSetErrorCallback registrieren
void error_callback(int error, const char *description);

// --- Eingabeverarbeitung ---
// Überprüft Tastaturereignisse (z.B. ESC-Taste zum Schließen des Fensters)
void processInput(GLFWwindow *window);

// --- OpenGL-Fehlerüberprüfung ---
// Diese Funktion nach jedem OpenGL-Befehl aufrufen, um Fehler zu erkennen
bool checkGLError(const char *file, int line);

// Definiert das Makro DEBUG_GL_CALL zur einfachen Verwendung von checkGLError
// Fügt automatisch Dateiname und Zeilennummer hinzu
#ifdef _DEBUG
#define GL_CALL(x)                        \
    do                                    \
    {                                     \
        x;                                \
        checkGLError(__FILE__, __LINE__); \
    } while (0)
#else
#define GL_CALL(x) x
#endif

// --- Datei lesen (für Shader oder andere Ressourcen) ---
// Liest den gesamten Inhalt einer Datei und gibt ihn als Zeichenkette zurück
// Der Aufrufer muss den Speicher anschließend mit free() freigeben
char *read_file(const char *filepath);

#endif // UTILS_H
