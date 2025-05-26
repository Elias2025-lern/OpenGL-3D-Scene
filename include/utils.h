#ifndef UTILS_H
#define UTILS_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>   // Für fprintf
#include <stdbool.h> // Für bool
#include <assert.h>  // Für assert

// Verarbeitet Benutzereingaben (z.B. ESC-Taste zum Schließen des Fensters)
void processInput(GLFWwindow *window);

// Liest den gesamten Inhalt einer Datei und gibt ihn als Zeichenkette zurück.
// Der Aufrufer ist verantwortlich, den Speicher nach der Nutzung mit free() freizugeben.
char *read_file(const char *filepath);

#endif // UTILS_H
