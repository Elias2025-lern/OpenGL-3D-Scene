#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Fenstergröße
const int WINDOW_WIDTH = 1920;
const int WINDOW_HEIGHT = 1080;
const char* WINDOW_TITLE = "Computergraphik - Projekt";

// Callback für GLFW-Fehler
void error_callback(int error, const char* description) {
    fprintf(stderr, "Fehler: %s\n", description);
}

// Tastatureingabe verarbeiten
void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main() {
    GLFWwindow* window;

    if (!glfwInit()) {
        fprintf(stderr, "GLFW konnte nicht initialisiert werden\n");
        exit(EXIT_FAILURE);
    }

    glfwSetErrorCallback(error_callback);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Fenster erstellen
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);
    if (!window) {
        fprintf(stderr, "GLFW-Fenster konnte nicht erstellt werden\n");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
 
    // GLEW initialisieren
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        fprintf(stderr, "GLEW konnte nicht initialisiert werden: %s\n", glewGetErrorString(err));
        glfwDestroyWindow(window);
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    fprintf(stdout, "Status: GLEW-Version %s wird verwendet\n", glewGetString(GLEW_VERSION));
    fprintf(stdout, "OpenGL-Version: %s\n", glGetString(GL_VERSION));
    fprintf(stdout, "GLSL-Version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

    // Render-Schleife
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
