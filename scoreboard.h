#ifndef SCOREBOARD_H
#define SCOREBOARD_H

#include <vector>

// ============================================================
//  Bestenlisten-Eintrag
// ============================================================

/// Ein einzelner Eintrag in der Bestenliste.
struct ScoreEntry {
    char  name[16];  // Spielername (max. 15 Zeichen + Nullterminator)
    int   score;     // Erreichte Punktzahl
    float time;      // Überlebte Zeit in Sekunden
};

// ============================================================
//  Funktionsdeklarationen
// ============================================================

/// Lädt alle Einträge aus scoreboard.dat, sortiert nach Score (absteigend).
std::vector<ScoreEntry> LoadScoreboard();

/// Fügt einen neuen Eintrag hinzu und speichert die Top 10.
/// Ist der Name leer, wird "Gast" verwendet.
void AddOrUpdateScore(const char* name, int score, float time);

/// Löscht scoreboard.dat vollständig.
void ClearScoreboard();

/// Gibt den höchsten gespeicherten Score zurück (0 falls keine Einträge).
int GetTopScore();

#endif // SCOREBOARD_H
