# 🧊 Comgraph3D

**Comgraph3D** ist ein 3D-Grafikprojekt, das grundlegende OpenGL-Techniken demonstriert, entwickelt in der Programmiersprache **C** unter Verwendung moderner OpenGL (Version 3). Es bietet eine einfache Plattform zur Visualisierung und Manipulation von 3D-Objekten.

## 🔧 Verwendete Technologien

- **C** – Systemnahe Programmiersprache für hohe Leistung
- **OpenGL 3** – Grafikbibliothek für 3D-Darstellung
- **GLEW** – OpenGL Extension Wrangler Library zur Verwaltung von Erweiterungen
- **GLFW** – plattformübergreifendes Fenster- und Eingabe-Handling

## 🚀 Demo

*(Hier können Sie Screenshots oder einen Link zu einer Demo einfügen)*

## 📁 Projektstruktur

```bash
Comgraph3D/
├── assets/           # Enthält Projekt-Ressourcen
│       ├── shaders/      # Shader-Quellcode (Vertex, Fragment, Geometrie...) (z.B. .vert, .frag)
│     	└── textures/     # Bilddateien für Texturen (z.B. .png, .jpg)      
├── build/            # Verzeichnis für kompilierte Dateien
├── docs/             # Projektdokumentation (optional)
├── include/          # Header-Dateien (.h) für die Module deines Projekts
├── src/              # C-Quellcodedateien (.c)
│   ├── camera.c      # Code für die Kamerabehandlung
│   ├── main.c        # Haupteinstiegspunkt der Anwendung und Hauptschleife
│   ├── mesh.c        # Code zum Laden und Behandeln von Meshes/Modellen
│   └── shader.c      # Code zum Laden, Kompilieren und Verwalten von Shadern
├── Makefile          # Konfigurationsdatei für das Make-Build-System
└── README.md         # Projektbeschreibung
```
## ⚙️ Installation
### Voraussetzungen
	-Linux oder WSL2 unter Windows
	-Compiler (z. B. GCC)
	-Make
	-GLEW und GLFW installiert (z. B. über Paketmanager):
	
```bash
# Für Debian/Ubuntu:
sudo apt install libglew-dev libglfw3-dev libglm-dev meson ninja-build
```

### Build & Ausführen

Befolge diese Schritte, um das Projekt zu klonen, zu bauen und die Anwendung zu starten:

1.  **Repository klonen:**
    Klone den Projekt-Code von GitHub auf deinen lokalen Rechner.
    ```bash
    git clone https://github.com/quylecse/comgraph3d.git
    ```

2.  **In das Projektverzeichnis wechseln:**
    Navigiere in den neu erstellten Projektordner.
    ```bash
    cd comgraph3d
    ```

3.  **Projekt bauen:**
    Verwende den `make`-Befehl, um das Projekt anhand des Makefiles im Wurzelverzeichnis zu kompilieren und zu linken.
    ```bash
    make
    ```

4.  **Anwendung starten:**
    Führe die kompilierte Anwendung aus. Der genaue Befehl oder Pfad zur ausführbaren Datei kann je nach Konfiguration deines Makefiles variieren. Passe den Pfad bei Bedarf an den tatsächlichen Speicherort der ausführbaren Datei an (z. B. `./bin/comgraph3d`, falls eine `bin`-Ordner erstellt wird).
    ```bash
    ./comgraph3d
    ```
    (Alternative: Wenn dein `Makefile` ein `run`-Target definiert, kannst du stattdessen auch einfach `make run` ausführen.)

## 👤 Autor

- **Phu Quy Le**, Matrikelnummer: 1764640
