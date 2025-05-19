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
.
├── src/            # Quellcode des Projekts
├── include/        # Header-Dateien
├── assets/         # Ressourcen (Texturen, Modelle)
├── build/          # Kompilierte Dateien
├── README.md       # Dieses Dokument
└── … 
```

## ⚙️ Installation
###Voraussetzungen
	-Linux oder WSL2 unter Windows
	-Compiler (z. B. GCC)
	-Meson und Ninja
	-GLEW und GLFW installiert (z. B. über Paketmanager):
	
```bash
# Für Debian/Ubuntu:
sudo apt install libglew-dev libglfw3-dev libglm-dev meson ninja-build
```

### Build & Ausführen
```bash
# Repository klonen
git clone [https://github.com/quylecse/comgraph3d.git](https://github.com/quylecse/comgraph3d.git)
cd comgraph3d

# Build-Verzeichnis erstellen
meson setup build

# Kompilieren
meson compile -C build

# Anwendung starten
./build/comgraph3d
```

## 👤 Autor

- **Phu Quy Le**, Matrikelnummer: 1764640
