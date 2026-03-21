#pragma once
#include <string>
#include <vector>

// Die Struktur für einen einzelnen Eintrag in der Bestenliste
struct ScoreEntry {
    std::string name;
    int level;
};

// Lade die Bestenliste aus der Textdatei
std::vector<ScoreEntry> LoadScoreboard();

// Speichere die Bestenliste in die Textdatei
void SaveScoreboard(const std::vector<ScoreEntry>& scores);

// Füge einen neuen Eintrag hinzu (oder aktualisiere das Level eines bestehenden Spielers)
void AddOrUpdateScore(std::string name, int level);