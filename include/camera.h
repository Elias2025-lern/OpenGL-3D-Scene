
#ifndef CAMERA_H
#define CAMERA_H

#include <stdbool.h>
#include <GLFW/glfw3.h>

// Kamera-Parameter (extern, damit main.c sie lesen kann)
extern float yaw;            // Yaw-Winkel (links/rechts)
extern float pitch;          // Pitch-Winkel (oben/unten)
extern float radius;         // Abstand zur Szene
extern float lastX;          // Letzte Maus-X-Position
extern float lastY;          // Letzte Maus-Y-Position
extern bool rotating;        // Wird die Kamera gerade gedreht?
extern bool firstMouse;      // Erste Bewegung nach Mausklick ignorieren

// Initialisiert die Callback-Funktionen für Maus und Scrollrad
void camera_setup_callbacks(GLFWwindow* window);

// Setzt die Kamera zurück auf die Standardansicht
void camera_reset();

// Berechnet die aktuelle Kameraposition (für View-Matrix)
void camera_get_position(float* outVec3);

#endif
