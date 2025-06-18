// camera.c – Steuerung der Benutzerkamera (Maus, Scrollrad, Reset)
#include "camera.h"
#include <GLFW/glfw3.h>
#define _USE_MATH_DEFINES
#include <math.h>
#define DEFAULT_YAW 30.0f
#define DEFAULT_PITCH 15.0f
#define DEFAULT_RADIUS 75.0f
#define DEFAULT_SPEED 0.003f
#define PITCH_MAX 89.0f
#define PITCH_MIN -89.0f
#define RADIUS_MIN 5.0f
#define RADIUS_MAX 150.0f

// Globale Kameraparameter
float yaw = DEFAULT_YAW;
float pitch = DEFAULT_PITCH;
float radius = DEFAULT_RADIUS;
float lastX = 400, lastY = 300;
bool rotating = false;
bool firstMouse = true;

// Grad zu Radiant-Konvertierung (für Kamerawinkel)
float glm_rad(float deg) {
    return deg * (float)M_PI / 180.0f;
}

/* Maus-Funktionen*/
// Maustaste gedrückt oder losgelassen
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT)
        rotating = (action == GLFW_PRESS);
}
// Wenn Maus sich bewegt, ändert Kamerawinkel beim Drehen
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (!rotating) { 
        firstMouse = true; 
        return; 
    }
    if (firstMouse) { 
        lastX = (float)xpos;
        lastY = (float)ypos;
        firstMouse = false; 
    }
    float xoffset = (float)(xpos - lastX);
    float yoffset = (float)(lastY - ypos);

    lastX = (float)xpos;
    lastY = (float)ypos;

    const float sensitivity = 0.2f;
    yaw += xoffset * sensitivity;
    pitch += yoffset * sensitivity;

    if (pitch > PITCH_MAX) pitch = 89.0f;
    if (pitch < PITCH_MIN) pitch = -89.0f;
}
// Mausrad bewegt sich (Zoom)
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    // xoffset wird nicht verwendet, da nur vertikales Scrollen
    radius -= (float)yoffset;

    if (radius < RADIUS_MIN) radius = RADIUS_MIN;
    if (radius > RADIUS_MAX) radius = RADIUS_MAX;
}

/*Kamera-Funktionen*/

// Setzt die Kamera zurück auf Ausgangswert
void camera_reset() {
    yaw = DEFAULT_YAW;
    pitch = DEFAULT_PITCH;
    radius = DEFAULT_RADIUS;
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
// Setzt die Kameraposition (View-Matrix) mit Tastatur
void camera_process_keyboard(GLFWwindow* window, float deltaTime) {
    float velocity = DEFAULT_SPEED * deltaTime * 10.0f;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        pitch += velocity;
        if (pitch > PITCH_MAX) pitch = 89.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        pitch -= velocity;
        if (pitch < PITCH_MIN) pitch = -89.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        yaw -= velocity;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        yaw += velocity;
    }
}

/*Kameraswerte-Getter*/
float camera_get_yaw() { return yaw; }
float camera_get_pitch() { return pitch; }
float camera_get_radius() { return radius; }
