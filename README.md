
# 🧊 Comgraph3D – Interaktives 3D-Sonnensystem

**Comgraph3D** ist ein interaktives 3D-Grafikprojekt zur Darstellung eines Sonnensystems.  
Es wurde in **C** mit OpenGL 3.3 umgesetzt und zeigt Planetenbewegung, Texturen, Licht und Kamerasteuerung.

---

## 🔧 Verwendete Technologien

- **C** – Systemnahe Programmiersprache
- **OpenGL 3.3 Core** – Grafikpipeline
- **GLEW** – Erweiterungsverwaltung
- **GLFW** – Fenster- und Eingabesteuerung
- **stb_image** – Bild- und Texturladung
- **Eigene Matrixfunktionen** – für Transformationen und Projektionen

---

## 🌍 Projekt-Features

- 3D-Sonnensystem mit Sonne + 9 Planeten  
- Echtzeit-Umlaufbahnen und Rotationen  
- Individuelle Texturen für jeden Planeten  
- Hintergrundbild (Weltraum)  
- 30 kleine und 3 große Asteroiden mit zufälligen Umläufen  
- Kamera-Rotation mit Maus, Zoom mit Mausrad, Reset mit Taste R  
- Lichtberechnung mit Phong-Shading  
- OBJ-Dateien für Planetenmodelle und Felsen

---

## 📁 Projektstruktur

```bash
Comgraph3D/
├── assets/
│   ├── shaders/
│   │   ├── basic_color.vert
│   │   └── basic_color.frag
│   └── textures/
│       ├── sun.jpg
│       ├── earth.jpg
│       ├── mars.jpg
│       ├── mercury.jpg
│       ├── venus.jpg
│       ├── jupiter.jpg
│       ├── saturn.jpg
│       ├── uranus.jpg
│       ├── neptune.jpg
│       ├── pluto.jpg
│       ├── rock.jpg
│       ├── large_rock.jpg
│       └── space.jpeg
├── models/
│   ├── cube.obj
│   └── rock.obj
├── include/
│   ├── camera.h
│   ├── camera_matrix.h
│   ├── light.h
│   ├── matrix.h
│   ├── matrix_transformation.h
│   ├── models.h
│   ├── object.h
│   ├── obj_loader.h
│   ├── shader.h
│   ├── stb_image.h
│   └── utils.h
├── src/
│   ├── main.c
│   ├── camera.c
│   ├── camera_matrix.c
│   ├── light.c
│   ├── matrix.c
│   ├── matrix_transformation.c
│   ├── models.c
│   ├── object.c
│   ├── obj_loader.c
│   ├── shader.c
│   └── test-matrix-und-camera.c
├── build/                   # Optional – für kompilierte Dateien
├── docs/                    # Optional – Projektdokumentation
├── obj/                     # Kompilierte Objektdateien (.o)
├── Makefile
├── README.md
├── app.exe

```

---

## ⚙️ Installation

### Voraussetzungen (MSYS2 empfohlen)

```bash
pacman -S mingw-w64-x86_64-gcc \
            mingw-w64-x86_64-glfw \
            mingw-w64-x86_64-glew \
            mingw-w64-x86_64-stb \
            make
```

---

### 🔧 Build & Start

```bash
make         # Projekt kompilieren
./app.exe    # Anwendung starten
```

---

## 🎮 Steuerung

| Eingabe         | Funktion                        |
|----------------|----------------------------------|
| Linksklick + Maus | Kamera rotieren                 |
| Mausrad         | Zoom                            |
| Taste `R`       | Kamera zurücksetzen             |

---

## ✅ Erfüllte Anforderungen

Alle Anforderungen aus dem Aufgabenblatt wurden umgesetzt:

1. **README-Datei** mit Anleitung und Beschreibung  
2. **Eigene Matrixfunktionen** für Transformationen  
3. **Import von .obj-Dateien**  
4. **Mehrere 3D-Objekte** (Sonne, Planeten, Felsen)  
5. **Animationen** von Kamera und Objekten  
6. **Phong-Lichtberechnung**  
7. **Texturierung** aller Planeten und Felsen  
8. **Benutzerinteraktion** (Maus & Tastatur)

---

## 👤 Autor:innen

- **Mohammed Al-Muliki**, Matrikelnummer: 1696172  
- **Phu Quy Le**, Matrikelnummer: 1764640  
- **Truong Minh Khoi Nguyen**, Matrikelnummer: 1764501  
