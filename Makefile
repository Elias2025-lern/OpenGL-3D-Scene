CC = gcc

# Kompilierungsoptionen
# -Wall: Alle Warnungen aktivieren
# -g: Debug-Informationen hinzufügen
# -v: Detaillierte Kompilierungsinformationen anzeigen (kann weggelassen werden, wenn nicht benötigt)
# -DGLEW_STATIC: SEHR WICHTIG für statisches Linken von GLEW (glew32s.lib)
# -Iinclude: Fügt den Ordner 'include' zum Suchpfad für Header-Dateien hinzu
CFLAGS = -Wall -g -v -DGLEW_STATIC -Iinclude

# Bibliotheken für das Linken (inklusive der von dir bereitgestellten Bibliotheken)
# -lglfw3: GLFW Bibliothek
# -lglew32: GLEW Bibliothek (meist glew32s.lib für statisches Linken)
# -lopengl32: OpenGL Bibliothek unter Windows
# -lgdi32 -luser32 -lkernel32: Windows-Systembibliotheken notwendig für OpenGL/GLFW
LIBS = -lglfw3 -lglew32 -lopengl32 -lgdi32 -luser32 -lkernel32

# Name der ausführbaren Datei des Projekts
TARGET = app.exe

# C-Quellcodedateien des Projekts
SRC_DIR = src
# WICHTIG: Sicherstellen, dass alle .c-Dateien hier aufgeführt sind!
SRCS = $(SRC_DIR)/main.c $(SRC_DIR)/utils.c $(SRC_DIR)/shader.c $(SRC_DIR)/matrix.c $(SRC_DIR)/models.c $(SRC_DIR)/object.c $(SRC_DIR)/obj_loader.c $(SRC_DIR)/light.c


# Ordner für Objektdateien (.o)
OBJ_DIR = obj
# Ersetze die .c Dateien in SRCS durch entsprechende .o Dateien im OBJ_DIR
OBJ = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

# Standard-Target: Erstelle die ausführbare Datei
all: $(OBJ_DIR) $(TARGET)

# Regel, um den Ordner OBJ_DIR zu erstellen, falls er noch nicht existiert
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Regel, um die Objektdateien zu einer ausführbaren Datei zu linken
# Abhängig von allen Objektdateien in $(OBJ)
# $(CC) $(CFLAGS) $(OBJ) -o $@ $(LIBS): Kompiliert und linked
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $@ $(LIBS)

# Regel zum Kompilieren von C-Quellcodedateien (.c) zu Objektdateien (.o)
# Dies ist eine Musterregel (pattern rule) für beliebige .c Dateien
# $<: Quellcodedatei (z.B. src/main.c)
# $@: Ziel-Objektdatei (z.B. obj/main.o)
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean-Target zum Löschen der erstellten Dateien während des Builds
clean:
	rm -f $(TARGET) $(OBJ)
	rmdir $(OBJ_DIR) 2> /dev/null || true # Löscht den obj-Ordner, ignoriert Fehler, wenn der Ordner nicht leer ist