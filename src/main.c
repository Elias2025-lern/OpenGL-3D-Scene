#include <GL/glew.h>          // OpenGL Extension Wrangler - für Erweiterungen
#include <GLFW/glfw3.h>       // GLFW - für Fenster- und Eingabehandling
#include <stdio.h>            // Standard-I/O-Funktionen
#include <stdlib.h>           // Standardbibliothek für Speicherverwaltung etc.
#define _USE_MATH_DEFINES
#include <math.h>             // Mathematische Funktionen
#include <time.h>             // Zeitfunktionen für Zufallszahlen

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"        // Bibliothek zum Laden von Bildern

#include "shader.h"           // Shader-Handling
#include "matrix.h"           // Matrizenoperationen
#include "object.h"           // 3D-Objekte
#include "obj_loader.h"       // OBJ-Dateien laden
#include "light.h"            // Lichtberechnungen

#define ROCK_TOTAL 30         // Anzahl der kleinen Felsbrocken
#define LARGE_ROCKS 3         // Anzahl der großen Felsbrocken

// Initiale Kameraparameter
const float INIT_YAW = 30.0f;    // Horizontale Ausrichtung
const float INIT_PITCH = 15.0f;  // Vertikale Ausrichtung
const float INIT_RADIUS = 75.0f; // Abstand vom Zentrum

// Globale Kameravariablen
float yaw = INIT_YAW;
float pitch = INIT_PITCH;
float radius = INIT_RADIUS;

// Maussteuerungsvariablen
float lastX = 400, lastY = 300;
bool firstMouse = true, rotating = false;

// Hintergrund-OpenGL-Objekte
GLuint bgVAO, bgVBO, bgTexture;

// Grad zu Bogenmaß Konvertierung
float glm_rad(float deg) {
    return deg * (float)M_PI / 180.0f;
}

// Struktur für Felsbrocken-Informationen
typedef struct {
    float angle_offset;   // Startwinkel
    float orbit_radius;   // Umlaufradius
    float scale;         // Skalierung
    float orbit_speed;   // Umlaufgeschwindigkeit
} RockInfo;

// Mausklick-Callback
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT)
        rotating = (action == GLFW_PRESS); // Rotation aktivieren bei Linksklick
}

// Mausbewegungs-Callback
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (!rotating) { firstMouse = true; return; } // Nur bei aktivierter Rotation
    if (firstMouse) { lastX = xpos; lastY = ypos; firstMouse = false; }

    // Differenz zur letzten Position berechnen
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos; lastY = ypos;

    // Kamerawinkel aktualisieren
    float sensitivity = 0.2f;
    yaw += xoffset * sensitivity;
    pitch += yoffset * sensitivity;

    // Vertikale Grenzen setzen
    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;
}

// Mausrad-Callback für Zoom
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    radius -= yoffset; // Kameradistanz ändern
    if (radius < 5.0f) radius = 5.0f;   // Minimale Distanz
    if (radius > 100.0f) radius = 100.0f; // Maximale Distanz
}

// Fenstergrößenänderungs-Callback
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height); // Viewport anpassen
}

// Textur laden
GLuint load_texture(const char* path) {
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // Texturparameter setzen
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Bild laden
    int w, h, c;
    unsigned char* data = stbi_load(path, &w, &h, &c, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        printf("Fehler beim Laden der Textur: %s\n", path);
    }
    stbi_image_free(data);
    return texture;
}

// Planeten aktualisieren und zeichnen
void update_planet(RenderObject* obj, GLuint tex, float radius, float speed, float scale, float time, Shader* shader, mat4* view, mat4* proj) {
    // Position berechnen
    float angle = time * speed;
    float x = radius * cos(angle);
    float z = radius * sin(angle);

    // Modelmatrix erstellen (Skalierung und Translation)
    obj->model_matrix = mat4_scale(mat4_translate(mat4_identity(), vec3_create(x, 0.0f, z)), vec3_create(scale, scale, scale));

    // Textur binden und zeichnen
    glBindTexture(GL_TEXTURE_2D, tex);
    glUniform1i(glGetUniformLocation(shader->id, "texture1"), 0);
    object_draw(obj, shader, view, proj);
}

int main() {
    // Initialisierung
    srand((unsigned int)time(NULL)); // Zufallsgenerator seeden
    if (!glfwInit()) return -1; // GLFW initialisieren

    // OpenGL Version und Profil setzen
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Fenster erstellen
    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL - Sonnensystem", NULL, NULL);
    if (!window) return -1;
    glfwMakeContextCurrent(window);

    // Callbacks registrieren
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    if (glewInit() != GLEW_OK) return -1; // GLEW initialisieren
    glEnable(GL_DEPTH_TEST); // Tiefentest aktivieren

    // Shader erstellen
    Shader shader = shader_create("shaders/basic_color.vert", "shaders/basic_color.frag");

    // Hintergrund-Geometrie (Fullscreen-Quad)
    float bgVertices[] = {
        -1.0f,  1.0f,  0.0f, 1.0f, // links oben
        -1.0f, -1.0f,  0.0f, 0.0f, // links unten
         1.0f, -1.0f,  1.0f, 0.0f, // rechts unten
        -1.0f,  1.0f,  0.0f, 1.0f, // links oben
         1.0f, -1.0f,  1.0f, 0.0f, // rechts unten
         1.0f,  1.0f,  1.0f, 1.0f  // rechts oben
    };

    // VAO und VBO für Hintergrund erstellen
    glGenVertexArrays(1, &bgVAO);
    glGenBuffers(1, &bgVBO);
    glBindVertexArray(bgVAO);
    glBindBuffer(GL_ARRAY_BUFFER, bgVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(bgVertices), bgVertices, GL_STATIC_DRAW);
    // Vertex-Attribute setzen
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    // Hintergrundtextur laden
    bgTexture = load_texture("textures/space.jpeg");

    // Planeten laden
    float* vertices = NULL;
    int num_floats = 0;
    load_obj("models/cube.obj", &vertices, &num_floats); // Würfel als Basis für Planeten

    RenderObject objs[10]; // 10 Objekte (Sonne + 9 Planeten)
    const char* textures[] = {
        "sun.jpg", "mercury.jpg", "venus.jpg", "earth.jpg", "mars.jpg",
        "jupiter.jpg", "saturn.jpg", "uranus.jpg", "neptune.jpg", "pluto.jpg"
    };
    GLuint tex_ids[10];

    // Objekte und Texturen initialisieren
    for (int i = 0; i < 10; ++i) {
        objs[i] = object_create(vertices, num_floats, 5); // 5 Werte pro Vertex (Position + Texturkoordinaten)
        char path[64];
        snprintf(path, sizeof(path), "textures/%s", textures[i]);
        tex_ids[i] = load_texture(path); // Textur laden
    }

    // Planetenparameter
    float radii[] = { 0.0f, 10.0f, 15.0f, 20.0f, 25.0f, 30.0f, 35.0f, 40.0f, 45.0f, 50.0f }; // Umlaufradien
    float speeds[] = { 0.0f, 2.5f, 1.8f, 1.2f, 0.9f, 0.6f, 0.5f, 0.4f, 0.35f, 0.25f }; // Umlaufgeschwindigkeiten
    float scales[] = { 3.0f, 0.45f, 0.5f, 0.6f, 0.55f, 0.9f, 0.85f, 0.76f, 0.72f, 0.5f }; // Skalierungen

    // Felsbrocken laden
    float* rock_vertices = NULL;
    int rock_floats = 0;
    load_obj("models/rock.obj", &rock_vertices, &rock_floats);
    RenderObject rocks[ROCK_TOTAL];
    for (int i = 0; i < ROCK_TOTAL; ++i) {
        rocks[i] = object_create(rock_vertices, rock_floats, 5);
    }
    GLuint rock_texture = load_texture("textures/rock.jpg");
    GLuint large_rock_texture = load_texture("textures/large_rock.jpg");

    // Felsbrockenparameter initialisieren
    RockInfo rock_infos[ROCK_TOTAL];
    for (int i = 0; i < ROCK_TOTAL; ++i) {
        rock_infos[i].angle_offset = ((float)rand() / RAND_MAX) * 2.0f * M_PI; // Zufälliger Startwinkel
        rock_infos[i].orbit_radius = 55.0f + ((float)rand() / RAND_MAX) * 25.0f; // Zufälliger Radius
        rock_infos[i].scale = 0.05f + ((float)rand() / RAND_MAX) * 0.1f; // Zufällige Skalierung
        rock_infos[i].orbit_speed = 0.1f + ((float)rand() / RAND_MAX) * 0.1f; // Zufällige Geschwindigkeit
    }

    // Große Felsbrocken
    RenderObject large_rocks[LARGE_ROCKS];
    RockInfo large_infos[LARGE_ROCKS];
    for (int i = 0; i < LARGE_ROCKS; ++i) {
        large_rocks[i] = object_create(rock_vertices, rock_floats, 5);
        large_infos[i].angle_offset = ((float)rand() / RAND_MAX) * 2.0f * M_PI;
        large_infos[i].orbit_radius = 60.0f + ((float)rand() / RAND_MAX) * 20.0f;
        large_infos[i].scale = 0.25f + ((float)rand() / RAND_MAX) * 0.1f;
        large_infos[i].orbit_speed = 0.2f + ((float)rand() / RAND_MAX) * 0.3f;
    }

    // Lichtquelle konfigurieren
    Light light = {
        .position = vec3_create(10.f, 10.0f, 10.0f),
        .color = vec3_create(1.5f, 1.5f, 1.5f),
        .ambient_strength = 0.2f,
        .specular_strength = 2.0f
    };

    // Hauptrenderloop
    while (!glfwWindowShouldClose(window)) {
        float time = (float)glfwGetTime(); // Zeit seit Start

        // Kamera zurücksetzen bei R-Taste
        if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
            yaw = INIT_YAW; pitch = INIT_PITCH; radius = INIT_RADIUS;
        }

        // Kameraposition berechnen (Kugelkoordinaten)
        float camX = radius * cosf(glm_rad(yaw)) * cosf(glm_rad(pitch));
        float camY = radius * sinf(glm_rad(pitch));
        float camZ = radius * sinf(glm_rad(yaw)) * cosf(glm_rad(pitch));
        vec3 camPos = vec3_create(camX, camY, camZ);

        // View- und Projektionsmatrizen erstellen
        mat4 view = mat4_lookAt(camPos, vec3_create(0, 0, 0), vec3_create(0, 1, 0));
        mat4 proj = mat4_perspective(glm_rad(45.0f), 800.0f / 600.0f, 0.1f, 200.0f);

        // Puffer löschen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Hintergrund zeichnen (ohne Tiefentest)
        glDisable(GL_DEPTH_TEST);
        shader_use(&shader);
        mat4 identity = mat4_identity();
        glUniformMatrix4fv(glGetUniformLocation(shader.id, "model"), 1, GL_FALSE, (float*)&identity);
        glUniformMatrix4fv(glGetUniformLocation(shader.id, "view"), 1, GL_FALSE, (float*)&identity);
        glUniformMatrix4fv(glGetUniformLocation(shader.id, "projection"), 1, GL_FALSE, (float*)&identity);
        glUniform1i(glGetUniformLocation(shader.id, "isBackground"), 1);
        glUniform1i(glGetUniformLocation(shader.id, "texture1"), 0);
        glBindTexture(GL_TEXTURE_2D, bgTexture);
        glBindVertexArray(bgVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
        glEnable(GL_DEPTH_TEST);

        // Shader für 3D-Objekte vorbereiten
        shader_use(&shader);
        glUniform1i(glGetUniformLocation(shader.id, "isBackground"), 0);
        light_set_uniforms(&shader, &light, camPos); // Lichtparameter setzen

        // Planeten zeichnen
        for (int i = 0; i < 10; ++i)
            update_planet(&objs[i], tex_ids[i], radii[i], speeds[i], scales[i], time, &shader, &view, &proj);

        // Kleine Felsbrocken zeichnen
        for (int i = 0; i < ROCK_TOTAL; ++i) {
            float angle = rock_infos[i].angle_offset + time * rock_infos[i].orbit_speed;
            float dist = rock_infos[i].orbit_radius;
            float x = dist * cosf(angle);
            float z = dist * sinf(angle);
            float y = sinf(angle * 2.0f + i) * 0.3f; // Leichte vertikale Variation

            // Modelmatrix erstellen
            rocks[i].model_matrix = mat4_scale(
                mat4_translate(mat4_identity(), vec3_create(x, y, z)),
                vec3_create(rock_infos[i].scale, rock_infos[i].scale, rock_infos[i].scale)
            );

            // Zeichnen
            glBindTexture(GL_TEXTURE_2D, rock_texture);
            glUniform1i(glGetUniformLocation(shader.id, "texture1"), 0);
            object_draw(&rocks[i], &shader, &view, &proj);
        }

        // Große Felsbrocken zeichnen
        for (int i = 0; i < LARGE_ROCKS; ++i) {
            float angle = large_infos[i].angle_offset + time * large_infos[i].orbit_speed;
            float dist = large_infos[i].orbit_radius;
            float x = dist * cosf(angle);
            float z = dist * sinf(angle);
            float y = sinf(angle * 1.5f + i) * 1.0f; // Stärkere vertikale Variation

            // Modelmatrix erstellen
            large_rocks[i].model_matrix = mat4_scale(
                mat4_translate(mat4_identity(), vec3_create(x, y, z)),
                vec3_create(large_infos[i].scale, large_infos[i].scale, large_infos[i].scale)
            );

            // Zeichnen
            glBindTexture(GL_TEXTURE_2D, large_rock_texture);
            glUniform1i(glGetUniformLocation(shader.id, "texture1"), 0);
            object_draw(&large_rocks[i], &shader, &view, &proj);
        }

        // Puffer tauschen und Events verarbeiten
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Aufräumen
    for (int i = 0; i < 10; ++i) object_destroy(&objs[i]);
    for (int i = 0; i < ROCK_TOTAL; ++i) object_destroy(&rocks[i]);
    for (int i = 0; i < LARGE_ROCKS; ++i) object_destroy(&large_rocks[i]);
    shader_destroy(&shader);
    free(vertices);
    free(rock_vertices);
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
