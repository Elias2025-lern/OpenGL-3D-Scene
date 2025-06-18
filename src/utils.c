#include "utils.h"
#include <stdlib.h> // Für malloc, free, exit

// Verarbeitet Benutzereingaben (z.B. ESC-Taste zum Schließen des Fensters)
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

// Liest den gesamten Inhalt einer Datei und gibt ihn als String zurück.
// Der Aufrufer ist verantwortlich, den Speicher nach der Verwendung mit free() freizugeben.
char* read_file(const char* filepath)
{
    assert(filepath != NULL && "Dateipfad darf nicht NULL sein");

    FILE* file = fopen(filepath, "rb"); // Öffne Datei im binären Lesemodus
    if (!file)
    {
        fprintf(stderr, "Konnte Datei nicht öffnen: %s\n", filepath);
        return NULL;
    }

    fseek(file, 0, SEEK_END);  // Verschiebe Zeiger ans Ende der Datei
    long length = ftell(file); // Ermittle aktuelle Position (Dateigröße)
    fseek(file, 0, SEEK_SET);  // Verschiebe Zeiger zurück zum Anfang der Datei

    char* buffer = (char*)malloc(length + 1); // Speicher reservieren
    if (!buffer)
    {
        fprintf(stderr, "Speicherreservierung für Datei fehlgeschlagen: %s\n", filepath);
        fclose(file);
        return NULL;
    }

    fread(buffer, 1, length, file); // Lese gesamten Dateiinhalt in den Puffer
    buffer[length] = '\0';          // Stelle sicher, dass String nullterminiert ist

    fclose(file);
    return buffer;
}
