# 🏎️ Car Race

Ein rasantes 2D-Autorennen, entwickelt mit **C++** und der **raylib** Library. Weiche Hindernissen aus, sammle Sterne und schalte neue Fahrzeuge im Shop frei!

## ✨ Features

- **Dynamisches Gameplay:** Die Geschwindigkeit erhöht sich, je länger du überlebst.
- **Car Shop:** Besuche den Shop, um mit gesammelten Sternen neue Skins bzw. Autos freizuschalten.
- **Scoreboard:** Deine Bestzeiten und Punkte werden lokal gespeichert.
- **Smart UI:** Ein HUD, das Punkte, Zeit, sowie Sterne in Echtzeit wiedergibt.
- **Multi-Language:** Unterstützung für Deutsch und Englisch.

## 🛠️ Technische Highlights

- **Custom Rendering:** Nutzt `RenderTexture2D` für eine perfekte Skalierung in jedem Fensterformat (Vollbild/Fenster) ohne Verzerrung.
- **Speichersystem:** Automatisches Laden und Speichern von Spielständen und Einstellungen.
- **Performance:** Ressourcen-schonendes Rendering mit 60 FPS Zielrate.

## 🚀 Installation & Start

### Voraussetzungen
* Ein C++ Compiler
* [CMake](https://cmake.org/) (Version 3.10 oder höher)

### Build-Anleitung
1. Klone das Repository oder lade den Code herunter.
2. Öffne den Ordner in deinem bevorzugten Editor (z.B. VS Code).
3. Nutze das CMake-Plugin, um das Projekt zu konfigurieren und zu bauen.
4. Die Assets werden beim Build automatisch in den `build`-Ordner kopiert.

## 🎮 Steuerung

- **Pfeiltasten Links/Rechts:** Auto steuern
- **Taste P:** Spiel pausieren
- **Taste ESC:** Zurück zum Menü / Spiel beenden
- **Enter:** Namen bestätigen

## 📂 Projektstruktur

* `main.cpp` - Der Zündschlüssel des Spiels.
* `game.cpp/h` - Die zentrale Spiellogik (Game Loop).
* `ui.cpp/h` - Alles rund um Menüs, Buttons und das Werkstatt-Design.
* `player.cpp/h` - Steuerung und Physik des Fahrzeugs.
* `assets/` - Alle Grafiken (Autos, Sterne, Hindernisse).