[🇩🇪 Deutsch](#deutsch) | [🇬🇧 English](#english)

---

#  Car Race <a name="deutsch"></a>

Ein rasantes 2D-Autorennen, entwickelt mit **C++** und der **raylib** Library. Weiche Hindernissen aus, sammle Sterne und schalte neue Fahrzeuge im Shop frei!

##  Features

- **Dynamisches Gameplay:** Die Geschwindigkeit erhöht sich, je länger du überlebst.
- **Car Shop:** Besuche den Shop, um mit gesammelten Sternen neue Autos freizuschalten.
- **Scoreboard:** Deine Bestzeiten und Punkte werden lokal gespeichert – mit tabellarischer Ansicht.
- **Smart UI:** Ein HUD, das Punkte, Zeit sowie Sterne in Echtzeit wiedergibt.
- **Multi-Language:** Unterstützung für Deutsch und Englisch.
- **Uhr-Buff:** Sammle eine Uhr ein, um die Spielgeschwindigkeit für 3 Sekunden zu verlangsamen.
- **Schild-Buff:** Sammle ein Schild ein, um für 5 Sekunden unverwundbar zu sein.
- **Wüsten-Modus:** Ab 5.000 Punkten wechselt die Umgebung von der Waldstraße zur Wüste.
- **Weltall-Modus:** Ab 10.000 Punkten wechselt die Umgebung ins Weltall – mit Regenbogenstraße, UFOs und Planeten.
- **Effekte-Schutz (Epilepsie):** Die blinkenden Weltall-Effekte können in den Einstellungen und im Pause-Menü deaktiviert werden.
- **Pause-Menü (ESC):** Vollständiges Pause-Menü mit Weiter, Zurück zum Menü, Lautstärkeregler, Effekte-Toggle und Spiel beenden.
- **Bestätigungsdialoge:** Kritische Aktionen (Menü verlassen, Spiel beenden, Name ändern, Daten löschen) werden mit einem Ja/Nein-Dialog abgesichert.
- **Sound-Kontrolle:** Lautstärke in den Einstellungen und im Pause-Menü anpassbar. Einstellung wird gespeichert. Musik stoppt beim Absturz.

##  Steuerung

| Taste | Aktion |
|---|---|
| **Pfeiltaste Links / Rechts** | Auto steuern |
| **ESC** (im Spiel) | Pause-Menü öffnen |
| **ESC** (im Pause-Menü) | Spiel fortsetzen |
| **ESC** (im Hauptmenü) | Spiel beenden (mit Bestätigung) |
| **Enter** | Namen bestätigen |

### Pause-Menü (ESC im Spiel)

| Option | Beschreibung |
|---|---|
| **Weiter** | Spiel fortsetzen |
| **Zurück zum Menü** | Ins Hauptmenü (Bestätigung erforderlich) |
| **Lautstärke** | Musik-Lautstärke per Slider anpassen |
| **Effekte: AN/AUS** | Blinkeffekte im Weltall-Modus umschalten |
| **Spiel beenden** | Anwendung beenden (Bestätigung erforderlich) |

##  Einstellungen

- **Sprache:** Deutsch / Englisch umschalten
- **Vollbild:** Fenster- oder Vollbildmodus
- **Effekte:** Blinkende Weltall-Effekte aktivieren / deaktivieren (Epilepsie-Schutz)
- **Lautstärke:** Musik-Lautstärke per Slider einstellen (wird gespeichert)
- **Name ändern:** Spielernamen neu eingeben (Bestätigung erforderlich)
- **Daten löschen:** Löscht Name, Bestenliste und alle Sterne & Autos (Bestätigung erforderlich)

##  Umgebungen

| Punkte | Umgebung | Besonderheit |
|---|---|---|
| 0 – 4.999 | 🌲 Waldstraße | Standard-Umgebung mit Bäumen |
| 5.000 – 9.999 | 🏜️ Wüste | Kakteen, gelbe Fahrbahnmarkierungen |
| 10.000+ | 🚀 Weltall | Regenbogenstraße, UFOs, Planeten, leuchtende Ränder |

>  **Epilepsie-Hinweis:** Der Weltall-Modus enthält blinkende Farbeffekte. Diese können in den Einstellungen deaktiviert werden.

##  Projektstruktur

* `main.cpp` – Der Einstiegspunkt des Spiels.
* `game.cpp/h` – Zentrale Spiellogik (Game Loop, Zustände, Eingabe).
* `ui.cpp/h` – Alle Menüs, Buttons, HUD und Bestätigungsdialoge.
* `player.cpp/h` – Steuerung und Physik des Fahrzeugs.
* `config.cpp/h` – Einstellungen, Spielstand-Persistenz und Speed-Berechnung.
* `scoreboard.cpp/h` – Highscore-Verwaltung (lokal gespeichert als `scoreboard.dat`).
* `tests.cpp` – Unit-Tests für die Spielmechanik (Geschwindigkeitslogik).
* `assets/` – Alle Grafiken (Autos, Sterne, Hindernisse, Uhr, Schild) – **nicht im Repo enthalten**.
* `sounds/` – Musikdatei (`soundtrack.mp3`) und Crash-Sound (`crash.mp3`) – **nicht im Repo enthalten**.
* `raylib/` – Mitgelieferte raylib-Bibliothek (kein separater Download nötig).

## Drittleistungen:
- Programmiert in VS Code
- GitHub als Softwareentwicklungsplattform (Repository)
- Claude Code als KI-Assistent für das Schreiben und Debuggen des C++-Codes
- Raylib als C-Bibliothek für 2D/3D-Spieleentwicklung (zlib-Lizenz)
- Google Gemini für Bildgenerierung

## Quellen:

//Assets
- Autos: https://looneybits.itch.io/2d-urban-cars
- Hindernis: https://gamedeveloperstudio.itch.io/animated-spikes
- Stern: https://pix3lcat.itch.io/star-collectibles
- Icon: Generiert von Google Gemini
- Uhr: https://cocodog.itch.io/pixel-clock-2d
- Schild: https://tekilor.itch.io/pixel-shield-pack-1

//Sounds
- Musik: https://www.youtube.com/watch?v=na_YhseAgBs
- Hindernis: https://pixabay.com/users/u_mgq59j5ayf-50948372/

---

#  Car Race <a name="english"></a>

A fast-paced 2D car racing game built with **C++** and the **raylib** library. Dodge obstacles, collect stars, and unlock new cars in the shop!

##  Features

- **Dynamic Gameplay:** Speed increases the longer you survive.
- **Car Shop:** Spend collected stars to unlock new car skins.
- **Scoreboard:** Your best times and scores are saved locally – displayed as a clean table.
- **Smart UI:** A HUD displaying score, time, and stars in real time.
- **Multi-Language:** Supports German and English.
- **Clock Buff:** Pick up a clock to slow down the game speed for 3 seconds.
- **Shield Buff:** Pick up a shield to become invincible for 5 seconds.
- **Desert Mode:** After 5,000 points the environment switches from forest road to desert.
- **Space Mode:** After 10,000 points the environment changes to outer space – with a rainbow road, UFOs, and planets.
- **Effects Toggle (Epilepsy Safety):** The flashing space effects can be disabled in Settings and the Pause Menu.
- **Pause Menu (ESC):** Full pause menu with resume, back to menu, volume slider, effects toggle, and quit game.
- **Confirmation Dialogs:** Critical actions (leave menu, quit game, change name, delete data) require a yes/no confirmation.
- **Sound Control:** Volume adjustable in Settings and Pause Menu. Setting is saved. Music stops on crash.

##  Controls

| Key | Action |
|---|---|
| **Arrow Left / Right** | Steer the car |
| **ESC** (in game) | Open pause menu |
| **ESC** (in pause menu) | Resume game |
| **ESC** (in main menu) | Quit game (with confirmation) |
| **Enter** | Confirm name |

### Pause Menu (ESC in game)

| Option | Description |
|---|---|
| **Resume** | Continue the game |
| **Back to Menu** | Return to main menu (confirmation required) |
| **Volume** | Adjust music volume via slider |
| **Effects: ON/OFF** | Toggle flashing effects in space mode |
| **Quit Game** | Exit the application (confirmation required) |

##  Settings

- **Language:** Toggle between German and English
- **Fullscreen:** Switch between windowed and fullscreen mode
- **Effects:** Enable / disable flashing space effects (epilepsy safety)
- **Volume:** Adjust music volume via slider (saved persistently)
- **Change Name:** Re-enter your player name (confirmation required)
- **Delete Data:** Deletes name, scoreboard, and all stars & cars (confirmation required)

##  Environments

| Points | Environment | Details |
|---|---|---|
| 0 – 4,999 | 🌲 Forest Road | Default environment with trees |
| 5,000 – 9,999 | 🏜️ Desert | Cacti, yellow road markings |
| 10,000+ | 🚀 Space | Rainbow road, UFOs, planets, glowing edges |

>  **Epilepsy Warning:** Space mode contains flashing color effects. These can be disabled in Settings.

##  Project Structure

* `main.cpp` – Entry point of the game.
* `game.cpp/h` – Core game logic (game loop, states, input).
* `ui.cpp/h` – All menus, buttons, HUD, and confirmation dialogs.
* `player.cpp/h` – Vehicle controls and physics.
* `config.cpp/h` – Settings, save game persistence, and speed calculation.
* `scoreboard.cpp/h` – Highscore management (saved locally as `scoreboard.dat`).
* `tests.cpp` – Unit tests for game mechanics (speed logic).
* `assets/` – All graphics (cars, stars, obstacles, clock, shield) – **not included in the repo**.
* `sounds/` – Music file (`soundtrack.mp3`) and crash sound (`crash.mp3`) – **not included in the repo**.
* `raylib/` – Bundled raylib library (no separate download needed).

## Third-Party Services:

- Developed in VS Code
- GitHub as software development platform (repository)
- Claude Code as AI assistant for writing and debugging the C++ code
- Raylib as C library for 2D/3D game development (zlib license)
- Google Gemini for generating icon

## Sources:

//Assets
- Cars: https://looneybits.itch.io/2d-urban-cars
- Obstacle: https://gamedeveloperstudio.itch.io/animated-spikes
- Star: https://pix3lcat.itch.io/star-collectibles
- Icon: Generated by Google Gemini
- Clock: https://cocodog.itch.io/pixel-clock-2d
- Shield: https://tekilor.itch.io/pixel-shield-pack-1

//Sounds
- Music: https://www.youtube.com/watch?v=na_YhseAgBs
- Crash: https://pixabay.com/users/u_mgq59j5ayf-50948372/
