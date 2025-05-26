#include "utils.h"
#include <stdlib.h> // For malloc, free, exit

// Process user input (e.g., ESC key to close window)
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

// Reads the entire content of a file and returns it as a string.
// The caller is responsible for freeing the memory using free() after use.
char *read_file(const char *filepath)
{
    assert(filepath != NULL && "Filepath cannot be NULL");

    FILE *file = fopen(filepath, "rb"); // Open file in binary read mode
    if (!file)
    {
        fprintf(stderr, "Failed to open file: %s\n", filepath);
        return NULL;
    }

    fseek(file, 0, SEEK_END);  // Move pointer to end of file
    long length = ftell(file); // Get current position (file size)
    fseek(file, 0, SEEK_SET);  // Move pointer back to beginning of file

    char *buffer = (char *)malloc(length + 1); // Allocate memory
    if (!buffer)
    {
        fprintf(stderr, "Failed to allocate memory for file: %s\n", filepath);
        fclose(file);
        return NULL;
    }

    fread(buffer, 1, length, file); // Read entire file content into buffer
    buffer[length] = '\0';          // Ensure string is null-terminated

    fclose(file);
    return buffer;
}