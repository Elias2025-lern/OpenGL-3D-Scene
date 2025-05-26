#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>   // For printf, fprintf
#include <stdlib.h>  // For exit()
#include <stdbool.h> // For bool

#include "utils.h"  // Include utility functions
#include "shader.h" // Include shader module

const char *WINDOW_TITLE = "OpenGL";

// --- Define data for a triangle with per-vertex colors ---
// Each vertex has: 3 position values (X, Y, Z) and 3 color values (R, G, B)
// Layout: PosX, PosY, PosZ, ColorR, ColorG, ColorB
float vertices[] = {
    // Position (X, Y, Z)  // Color (R, G, B)
    -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // Bottom-left (Red)
    0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,  // Bottom-right (Green)
    0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f    // Top-center (Blue)
};

// VAO and VBO for our triangle
GLuint VAO, VBO;

// Our shader object
Shader basicColorShader;

// --- Function to set up OpenGL buffers (VAO, VBO) ---
bool setup_buffers()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Configure Vertex Attribute (Position):
    // layout (location = 0) in shader is for position (aPos)
    // 3 floats per position, not normalized
    // stride is 6 * sizeof(float) because each vertex has 6 floats (3 pos + 3 color)
    // offset is 0 (starts from the beginning of the vertex data)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // Configure Vertex Attribute (Color):
    // layout (location = 1) in shader is for color (aColor)
    // 3 floats per color, not normalized
    // stride is 6 * sizeof(float) (total size of one vertex)
    // offset is 3 * sizeof(float) (starts after the 3 position floats)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0); // Unbind VAO to prevent accidental modification
    return true;
}

// --- Main Program Function ---
int main()
{
    GLFWwindow *window;

    // 1. Initialize GLFW
    // NOT registering an error callback with GLFW, as requested.
    if (!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return EXIT_FAILURE;
    }

    // 2. Configure GLFW and OpenGL Context
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // Khi chạy fullscreen, không cần tắt GLFW_RESIZABLE.
    // GLFW sẽ tự động quản lý kích thước.
    // glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // 3. Create GLFW Window - CHANGED FOR FULLSCREEN
    // Lấy monitor chính (màn hình chính của hệ thống)
    GLFWmonitor *primaryMonitor = glfwGetPrimaryMonitor();
    if (!primaryMonitor)
    {
        fprintf(stderr, "Failed to get primary monitor!\n");
        glfwTerminate();
        return EXIT_FAILURE;
    }

    // Lấy chế độ video hiện tại của monitor chính
    const GLFWvidmode *mode = glfwGetVideoMode(primaryMonitor);
    if (!mode)
    {
        fprintf(stderr, "Failed to get video mode of primary monitor!\n");
        glfwTerminate();
        return EXIT_FAILURE;
    }

    // Tạo cửa sổ toàn màn hình
    // Kích thước cửa sổ sẽ bằng độ phân giải hiện tại của màn hình
    // Tham số thứ tư là monitor, truyền primaryMonitor để bật fullscreen
    window = glfwCreateWindow(mode->width, mode->height, WINDOW_TITLE, primaryMonitor, NULL);
    if (!window)
    {
        fprintf(stderr, "Failed to create GLFW window\n");
        glfwTerminate(); // Free GLFW resources before exiting
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window); // Make the window's context current for OpenGL operations

    // 4. Initialize GLEW
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        fprintf(stderr, "Failed to initialize GLEW: %s\n", glewGetErrorString(err));
        glfwDestroyWindow(window); // Destroy window
        glfwTerminate();           // Free GLFW
        return EXIT_FAILURE;
    }

    // Print OpenGL and GLSL versions (optional, for verification)
    printf("OpenGL Version: %s\n", glGetString(GL_VERSION));
    printf("GLSL Version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

    // 5. Set up OpenGL buffers for the triangle
    if (!setup_buffers())
    {
        fprintf(stderr, "Failed to setup OpenGL buffers!\n");
        glfwDestroyWindow(window);
        glfwTerminate();
        return EXIT_FAILURE;
    }

    // 6. Create shader program from files
    basicColorShader = shader_create("shaders/basic_color.vert", "shaders/basic_color.frag");
    if (basicColorShader.id == 0)
    { // Check if shader creation was successful
        fprintf(stderr, "Failed to create shader program!\n");
        // Clean up buffers before exiting if shader creation failed
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glfwDestroyWindow(window);
        glfwTerminate();
        return EXIT_FAILURE;
    }

    // 7. Main Render Loop
    // Loop continues until the window is requested to close
    while (!glfwWindowShouldClose(window))
    {
        // Process user input (e.g., ESC key), defined in utils.c
        processInput(window);

        // --- Render Commands ---
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // Set background color (dark teal)
        glClear(GL_COLOR_BUFFER_BIT);         // Clear only the color buffer (no depth test needed for simple 2D triangle)

        // Use our shader program
        shader_use(&basicColorShader);
        // Bind the Vertex Array Object (VAO) to tell OpenGL which data to use
        glBindVertexArray(VAO);
        // Draw the triangle using 3 vertices (GL_TRIANGLES primitive type)
        glDrawArrays(GL_TRIANGLES, 0, 3);
        // Unbind VAO to prevent unintended modification
        glBindVertexArray(0);

        // --- End Render Commands ---

        glfwSwapBuffers(window); // Swap front and back buffers to display the rendered frame
        glfwPollEvents();        // Process pending events (keyboard, mouse, window resize)
    }

    // 8. Clean up Resources
    fprintf(stdout, "Closing window and cleaning up...\n");
    // Free OpenGL resources
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    shader_destroy(&basicColorShader); // Free the shader program
    // Free GLFW resources
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0; // Exit program successfully
}