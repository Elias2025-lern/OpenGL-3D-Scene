
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

// Grad zu Radiant-Konvertierung (für Kamerawinkel)
float glm_rad(float deg);

// Maustaste gedrückt oder losgelassen
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

// Wenn Maus sich bewegt, ändert Kamerawinkel beim Drehen
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

// Mausrad bewegt sich (Zoom)
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

// Initialisiert die Callback-Funktionen fuer Maus und Scrollrad
void camera_setup_callbacks(GLFWwindow* window);

// Setzt die Kamera zurück auf die Standardansicht
void camera_reset();

// Berechnet die aktuelle Kameraposition (View-Matrix)
void camera_get_position(float* outVec3);

// Setzt die Kameraposition (View-Matrix) mit Tastatur
void camera_process_keyboard(GLFWwindow* window, float deltaTime);

// Kamera-Getter
float camera_get_yaw();
float camera_get_pitch();
float camera_get_radius();

#endif
