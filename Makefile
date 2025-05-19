# Betriebssystem erkennen
OS := $(shell uname)

GLEW_LIBS := $(shell pkgconf --libs glew)
GLFW_LIBS := $(shell pkgconf --libs glfw3)

# Libraries für Linux setzen
ifeq ($(OS), Linux)
	OPENGL_LIB := -lGL
# Libraries Windows (MSYS2) setzen
else
	OPENGL_LIB := -lopengl32
endif

# Quelldateien
SRC := src/main.c src/noch-eine-datei.c src/noch-mehr-dateien.c

# Output binary
OUT := cg1

# Compiler
CC := gcc

# Build target
$(OUT): $(SRC)
	$(CC) -o $(OUT) $(SRC) $(GLEW_LIBS) $(GLFW_LIBS) $(OPENGL_LIB)

# Clean target
clean:
	rm -f $(OUT)

.PHONY: clean