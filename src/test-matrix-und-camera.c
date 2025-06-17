#include "camera.h"
#include "matrix_transformation.h"
#include <stdio.h>

GLuint program, vao, VBO, EBO;
void init(void) {
    // Vertex Shader
    const char* vertexText = "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "layout (location = 1) in vec3 aColor;\n"
        "out vec3 vColor;\n"
        "uniform mat4 model;\n"
        "uniform mat4 view;\n"
        "uniform mat4 projection;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
        "   vColor = aColor;\n"
        "}\n";
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexText, NULL);
    glCompileShader(vertexShader);
    GLint status;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
    if(!status) {
        printf("Error compiling vertex shader:");
        GLchar infoLog[1024];
        glGetShaderInfoLog(vertexShader, 1024, NULL, infoLog);
        printf("%s\n", infoLog);
    }

// Fragment Shader
    const char* fragmentText = "#version 330 core\n"
        "in vec3 vColor;\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "   FragColor = vec4(vColor, 1.0);\n"
        "}\n";
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentText, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
    if(!status) {
        printf("Error compiling fragment shader:");
        GLchar infoLog[1024];
        glGetShaderInfoLog(fragmentShader, 1024, NULL, infoLog);
        printf("%s\n", infoLog);
    }

    //create and link shader program
    program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if(!status) {
        printf("Error linking program:");
        GLchar infoLog[1024];
        glGetProgramInfoLog(program, 1024, NULL, infoLog);
        printf("%s\n", infoLog);
    }

    glValidateProgram(program);
    glGetProgramiv(program, GL_VALIDATE_STATUS, &status);
    if(!status) {
        printf("Error linking program:");
        GLchar infoLog[1024];
        glGetProgramInfoLog(program, 1024, NULL, infoLog);
        printf("%s\n", infoLog);
    }

    // Cube vertices: 8 vertices with positions and colors (each face has a unique color)
    GLfloat vertices[] = {
        // Positions        // Colors (R, G, B)
        // Front face (red)
        -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
        // Back face (green)
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
        // Left face (blue)
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
        // Right face (yellow)
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,
        // Top face (cyan)
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 1.0f,
        // Bottom face (magenta)
        -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.0f
    };

    // Cube indices (defining triangles with CCW winding)
    unsigned int indices[] = {
        // Front face
        0, 1, 2,  2, 3, 0,
        // Back face
        4, 6, 5,  6, 4, 7,
        // Left face
        8, 9, 10, 10, 11, 8,
        // Right face
        12, 14, 13, 14, 12, 15,
        // Top face
        16, 18, 17, 18, 16, 19,
        // Bottom face
        20, 21, 22, 22, 23, 20
    };

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glViewport(0,0,800,600);

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    /*glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);*/
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0,0, width, height);
}
void draw(void) {

    static float angle = 0.0f;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glUseProgram(program);
    glBindVertexArray(vao);


    // Get uniform locations
    int modelLoc = glGetUniformLocation(program, "model");
    int viewLoc = glGetUniformLocation(program, "view");
    int projLoc = glGetUniformLocation(program, "projection");

    // Print uniform locations for debugging
    //printf("modelLoc: %d, viewLoc: %d, projLoc: %d\n", modelLoc, viewLoc, projLoc);

    // Set up view and projection matrices
    GLfloat view[16];
    GLfloat eye[] = {0.0f, 0.0f, 3.0f}; // Moved closer from 5.0 to 3.0
    GLfloat center[] = {0.0f, 0.0f, 0.0f};
    GLfloat up[] = {0.0f, 1.0f, 0.0f};
    lookAt(view, eye, center, up);


    // Projection parameters (can be modified to observe effects)
    GLfloat projection[16];
    GLfloat fovy = 3.14159f / 2.0f; // 90 degrees for wider view
    GLfloat aspect = 800.0f / 600.0f;
    GLfloat near = 0.5f;
    GLfloat far = 10.0f;
    perspective(projection, fovy, aspect, near, far);

    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, view);
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, projection);

    //
    GLfloat model[16], temp[16], temp2[16];

    // Cube 1: Left
    identity(model);
    GLfloat trans1[] = {-2.0f, 0.0f, 0.0f}; // Adjusted from -5.0 for visibility
    translate(temp, model, trans1); // Translate first
    GLfloat scale1[] = {0.3f, 0.3f, 0.3f};
    scale(temp2, temp, scale1); // Scale
    rotatez(model, temp2, angle * 2); // Rotate last
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, model);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    
    // Cube 2: Center
    identity(model);
    GLfloat trans2[] = {0.0f, 0.0f, 0.0f};
    translate(temp, model, trans2);
    GLfloat scale2[] = {1.0f, 1.0f, 1.0f};
    scale(temp2, temp, scale2);
    rotatez(model, temp2, angle);
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, model);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


    // Cube 3: Right
    identity(model);
    GLfloat trans3[] = {2.0f, 0.0f, 0.0f};
    translate(temp, model, trans3);
    GLfloat scale3[] = {1.5f, 1.5f, 1.5f};
    scale(temp2, temp, scale3);
    rotatez(model, temp2, - angle + 0.2f);
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, model);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
       

    // Update animation
    angle += 0.01f;
    if (angle > 2.0f * 3.14159f) angle -= 2.0f * 3.14159f;

    // Check for OpenGL errors
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        printf("OpenGL error: %d\n", err);
    }

}
int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a window
    GLFWwindow* window = glfwCreateWindow(800, 600, "Colored Cubes", NULL, NULL);
    if (!window) {
        printf("Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwMakeContextCurrent(window);


    printf("OpenGL Version: %s\n", glGetString(GL_VERSION));

    glewInit();
    init();

    

    while(!glfwWindowShouldClose(window)) {
        draw();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(program);
    glfwTerminate();
    return 0;
}