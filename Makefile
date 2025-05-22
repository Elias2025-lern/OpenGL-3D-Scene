# Zielname der ausführbaren Datei
TARGET = app

# Verzeichnisse für Quellcode und Header-Dateien
SRCDIR = src
INCDIR = include

# Compiler und Compiler-Optionen
CC = gcc
CFLAGS = -Wall -std=c11 -I$(INCDIR)  # Warnungen aktivieren, C11 Standard, Include-Verzeichnis hinzufügen

# Linker-Optionen und Bibliotheken
LDFLAGS = -lglew32 -lglfw3 -lopengl32 -lgdi32  # GLEW, GLFW und Windows OpenGL/GDI Bibliotheken linken

# Alle C-Quelldateien im src-Verzeichnis finden
SRC = $(wildcard $(SRCDIR)/*.c)

# Objektdateien (nicht verwendet, aber definiert)
OBJ = $(SRC:.c=.o)

# Standardziel: die ausführbare Datei bauen
all: $(TARGET)

# Bauen der ausführbaren Datei aus den Quelldateien
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Aufräumen: ausführbare Datei löschen
clean:
	del /Q $(TARGET)
