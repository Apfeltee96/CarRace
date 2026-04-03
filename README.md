[🇩🇪 Deutsch](#deutsch) | [🇬🇧 English](#english)

---

# 🏎️ Car Race <a name="deutsch"></a>

Ein rasantes 2D-Autorennen, entwickelt mit **C++** und der **raylib** Library. Weiche Hindernissen aus, sammle Sterne und schalte neue Fahrzeuge im Shop frei!

## ✨ Features

- **Dynamisches Gameplay:** Die Geschwindigkeit erhöht sich, je länger du überlebst.
- **Car Shop:** Besuche den Shop, um mit gesammelten Sternen neue Skins bzw. Autos freizuschalten.
- **Scoreboard:** Deine Bestzeiten und Punkte werden lokal gespeichert.
- **Smart UI:** Ein HUD, das Punkte, Zeit, sowie Sterne in Echtzeit wiedergibt.
- **Multi-Language:** Unterstützung für Deutsch und Englisch.
- **Uhr-Buff:** Sammle eine Uhr ein, um die Spielgeschwindigkeit für 3 Sekunden zu verlangsamen – mehr Zeit, um Hindernissen auszuweichen und Punkte zu sammeln!

## 🛠️ Technische Highlights

- **Custom Rendering:** Nutzt `RenderTexture2D` für eine perfekte Skalierung in jedem Fensterformat (Vollbild/Fenster) ohne Verzerrung.
- **Speichersystem:** Automatisches Laden und Speichern von Spielständen und Einstellungen.
- **Performance:** Ressourcen-schonendes Rendering mit 60 FPS Zielrate.

## 🚀 Installation & Start

### Voraussetzungen
* Ein C++ Compiler (z.B. GCC, MSVC, Clang)
* [CMake](https://cmake.org/) (Version 3.10 oder höher)

### Build-Anleitung

**Via Terminal:**
```bash
git clone https://github.com/apfeltee96/carrace.git
cd carrace
mkdir build && cd build
cmake ..
cmake --build .
```

**Via VS Code:**
1. Öffne den Ordner in VS Code.
2. Installiere die CMake-Extension.
3. Führe `CMake: Configure` und anschließend `CMake: Build` aus.

> **Hinweis:** Die Assets (`assets/`-Ordner mit den Grafiken) sind **nicht** im Repository enthalten und müssen separat bereitgestellt werden. Lege den `assets/`-Ordner mit den folgenden Dateien im Projektverzeichnis ab, bevor du das Spiel startest:
> - `car_white.png`, `car_red.png`, `car_blue.png`
> - `hindernis.png`, `star.png`, `clock.png`

## 🎮 Steuerung

- **Pfeiltasten Links/Rechts:** Auto steuern
- **Taste P:** Spiel pausieren
- **Taste ESC:** Zurück zum Menü / Spiel beenden
- **Enter:** Namen bestätigen

## 📂 Projektstruktur

* `main.cpp` - Der Zündschlüssel des Spiels.
* `game.cpp/h` - Die zentrale Spiellogik (Game Loop).
* `ui.cpp/h` - Alles rund um Menüs, Buttons und das Laden-Design.
* `player.cpp/h` - Steuerung und Physik des Fahrzeugs.
* `config.cpp/h` - Einstellungen, Spielstand-Persistenz und Speed-Berechnung.
* `scoreboard.cpp/h` - Highscore-Verwaltung (lokal gespeichert als `scoreboard.dat`).
* `tests.cpp` - Unit-Tests für die Spielmechanik (Geschwindigkeitslogik).
* `assets/` - Alle Grafiken (Autos, Sterne, Hindernisse, Uhr) – **nicht im Repo enthalten**.
* `raylib/` - Mitgelieferte raylib-Bibliothek (kein separater Download nötig).


## Drittleistungen:
- Programmiert in VS Code
- GitHub als Softwareentwicklungsplattform (Repository)
- Claude code als KI-Assistent für das Schreiben und Debuggen des C++-Codes
- Raylib als C-Bibliothek für 2D/3D-Spieleentwicklung (zlib-Lizenz)
- Google Gemini für Bildgenerierung

## Quellen-Assets-PNG's:
- Autos: https://looneybits.itch.io/2d-urban-cars
- Hindernis: https://gamedeveloperstudio.itch.io/animated-spikes
- Stern: https://pix3lcat.itch.io/star-collectibles
- Icon: Generiert von Google Gemini
- Uhr: https://cocodog.itch.io/pixel-clock-2d

---

# 🏎️ Car Race <a name="english"></a>

A fast-paced 2D car racing game built with **C++** and the **raylib** library. Dodge obstacles, collect stars, and unlock new cars in the shop!

## ✨ Features

- **Dynamic Gameplay:** Speed increases the longer you survive.
- **Car Shop:** Spend collected stars to unlock new car skins.
- **Scoreboard:** Your best times and scores are saved locally.
- **Smart UI:** A HUD displaying score, time, and stars in real time.
- **Multi-Language:** Supports German and English.
- **Clock Buff:** Pick up a clock to slow down the game speed for 3 seconds – more time to dodge obstacles and rack up points!

## 🛠️ Technical Highlights

- **Custom Rendering:** Uses `RenderTexture2D` for perfect scaling in any window size (fullscreen/windowed) without distortion.
- **Save System:** Automatically loads and saves game progress and settings.
- **Performance:** Resource-efficient rendering targeting 60 FPS.

## 🚀 Installation & Getting Started

### Prerequisites
* A C++ compiler (e.g. GCC, MSVC, Clang)
* [CMake](https://cmake.org/) (version 3.10 or higher)

### Build Instructions

**Via Terminal (recommended):**
```bash
git clone https://github.com/apfeltee96/carrace.git
cd carrace
mkdir build && cd build
cmake ..
cmake --build .
```

**Via VS Code:**
1. Open the folder in VS Code.
2. Install the CMake extension.
3. Run `CMake: Configure` followed by `CMake: Build`.

> **Note:** The assets (`assets/` folder with graphics) are **not included** in the repository and must be provided separately. Place the `assets/` folder with the following files in the project directory before launching the game:
> - `car_white.png`, `car_red.png`, `car_blue.png`
> - `hindernis.png`, `star.png`, clock.png

## 🎮 Controls

- **Arrow Keys Left/Right:** Steer the car
- **Key P:** Pause the game
- **Key ESC:** Back to menu / quit game
- **Enter:** Confirm name

## 📂 Project Structure

* `main.cpp` - The entry point of the game.
* `game.cpp/h` - Core game logic (game loop).
* `ui.cpp/h` - All menus, buttons, and the shop-design.
* `player.cpp/h` - Vehicle controls and physics.
* `config.cpp/h` - Settings, save game persistence, and speed calculation.
* `scoreboard.cpp/h` - Highscore management (saved locally as `scoreboard.dat`).
* `tests.cpp` - Unit tests for game mechanics (speed logic).
* `assets/` - All graphics (cars, stars, obstacles, clock) – **not included in the repo**.
* `raylib/` - Bundled raylib library (no separate download needed).



## Third-Party Services:

- Developed in VS Code
- GitHub as software development platform (repository)
- Claude Code as AI assistant for writing and debugging the C++ code
- Raylib as C library for 2D/3D game development (zlib license)
- Google Gemini for generating icon

## Source Assets (PNGs):

- Cars: https://looneybits.itch.io/2d-urban-cars
- Obstacle: https://gamedeveloperstudio.itch.io/animated-spikes
- Star: https://pix3lcat.itch.io/star-collectibles
- Icon: Generated by Google Gemini
- Clock: https://cocodog.itch.io/pixel-clock-2d