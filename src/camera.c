// camera.c – Steuerung der Benutzerkamera (Maus, Scrollrad, Reset)
#include "camera.h"
#include <GLFW/glfw3.h>
#define _USE_MATH_DEFINES
#include <math.h>

// Globale Kameraparameterloat yaw = 0.0f, pitch = 0.0f, radius = 50.0f;
float lastX = 400, lastY = 300;
bool rotating = false, firstMouse = true;

// Grad zu Radiant-Konvertierung (für Kamerawinkel)
static float glm_rad(float deg) {
    return deg * (float)M_PI / 180.0f;
}

// Maustaste gedrückt oder losgelassen
static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT)
        rotating = (action == GLFW_PRESS);
}

// Maus bewegt sich
static void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (!rotating) { firstMouse = true; return; }
    if (firstMouse) { lastX = xpos; lastY = ypos; firstMouse = false; }
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // Y ist invertiert
    lastX = xpos;
    lastY = ypos;
    float sensitivity = 0.2f;
    yaw += xoffset * sensitivity;
    pitch += yoffset * sensitivity;
    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;
}

// Mausrad bewegt sich (Zoom)
static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    radius -= yoffset;
    if (radius < 5.0f) radius = 5.0f;
    if (radius > 150.0f) radius = 150.0f;
}

// Setzt die Kamera zurück auf Ausgangswert
void camera_reset() {
    yaw = 0.0f;
    pitch = 0.0f;
    radius = 50.0f;
}

// Initialisiert alle Callback-Funktionen
void camera_setup_callbacks(GLFWwindow* window) {
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
}

// Berechnet Kameraposition für LookAt
void camera_get_position(float* outVec3) {
    float camX = radius * cosf(glm_rad(yaw)) * cosf(glm_rad(pitch));
    float camY = radius * sinf(glm_rad(pitch));
    float camZ = radius * sinf(glm_rad(yaw)) * cosf(glm_rad(pitch));
    outVec3[0] = camX;
    outVec3[1] = camY;
    outVec3[2] = camZ;
}
