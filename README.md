# CarRace

# CarRace 🏎️

Ein in C++ entwickeltes 2D-Desktopspiel, bei dem Reaktionsgeschwindigkeit gefragt ist. Der Spieler steuert ein Auto und muss endlos herabfallenden Hindernissen ausweichen. Das Ziel ist es, mindestens 60 Sekunden zu überleben und den eigenen Highscore zu knacken.

Den Quellcode zu diesem Projekt findest du auch auf GitHub: [github.com/Apfeltee96/CarRace](https://github.com/Apfeltee96/CarRace)

## 🎮 Spielmechanik & Features (Algorithmen)
* **Kollisionserkennung:** Ein Algorithmus prüft in jedem Frame, ob sich die Hitboxen (Rechtecke) des Spielers und der Hindernisse überschneiden.
* **Dynamischer Schwierigkeitsgrad (Scaling):** Ein Algorithmus berechnet basierend auf der vergangenen Spielzeit die Fallgeschwindigkeit der Hindernisse. Alle 10 Sekunden wird das Spiel spürbar schneller.
* **Persistentes Highscore-System:** Das Spiel nutzt C++ File-Streams (`std::ifstream` / `std::ofstream`), um die beste Überlebenszeit in eine lokale `.txt`-Datei zu schreiben und beim Neustart wieder auszulesen.

## 🕹️ Steuerung
* **Pfeiltaste Links:** Auto nach links bewegen
* **Pfeiltaste Rechts:** Auto nach rechts bewegen
* **ENTER:** Spiel im Menü oder nach einem Game Over starten/neustarten

## 🛠️ Erfüllung der Projekt-Anforderungen
Dieses Projekt wurde als Prüfungs-/Abgabeleistung entwickelt und erfüllt folgende Kriterien:
1. **C++:** Die gesamte Logik ist in modernem C++ geschrieben.
2. **Desktopanwendung oder Spiel:** Es handelt sich um ein ausführbares 2D-Spiel für den Desktop.
3. **Einbindung mind. einer Library:** Es wurde die Bibliothek *Raylib* für das Rendering und die Eingabeverarbeitung eingebunden.
4. **Mind. ein Algorithmus oder Gamemechanik:** Implementiert wurden Kollisionsabfragen, prozedurale Schwierigkeitssteigerung und ein Datei-basiertes Highscore-System (siehe oben).

## 📦 Drittleistungen & Bibliotheken
* **Raylib:** Für die Fenstererstellung, das Zeichnen der 2D-Grafiken (Rechtecke, Text) und das Auslesen der Tastatureingaben wurde das Open-Source Framework [Raylib](https://www.raylib.com/) verwendet.
* **C++ Standardbibliothek:** Für Dateioperationen (`<fstream>`) und Datenstrukturen (`<vector>`).

## 🚀 Kompilieren und Ausführen (Build)
Ein fertiger Build für **[Bitte hier eintragen, z.B. Windows 64-bit]** liegt der Abgabe als `.exe`-Datei bei. 
Ein Doppelklick auf die ausführbare Datei reicht aus, um das Spiel zu starten. 

Um das Projekt aus dem Quellcode neu zu kompilieren, wird **CMake** sowie ein C++ Compiler (z.B. MinGW oder MSVC) benötigt.