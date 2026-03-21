#pragma once // Wichtig: Verhindert, dass C++ die Datei aus Versehen doppelt lädt

// Diese Struktur hält alle Einstellungen für ein einzelnes Level
struct LevelData {
    int levelNumber;
    float targetTime;      // Wie lange muss man überleben? (z.B. 60 Sekunden)
    float baseSpeed;       // Startgeschwindigkeit der Hindernisse
    float speedMultiplier; // Wie stark steigt die Geschwindigkeit über Zeit an?
};

// Diese Funktion rufen wir später auf, um die Level-Einstellungen zu holen
LevelData GetLevelData(int level);