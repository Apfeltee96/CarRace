#define _CRT_SECURE_NO_WARNINGS
#include "scoreboard.h"
#include <algorithm>
#include <fstream>
#include <cstring>
#include <cstdio> // Für std::remove

const char* SCORE_FILE = "scoreboard.dat";

// Hilfsfunktion zum Sortieren
bool CompareScores(const ScoreEntry& a, const ScoreEntry& b) {
    return a.score > b.score; 
}

// Lädt die Bestenliste aus der Datei
std::vector<ScoreEntry> LoadScoreboard() {
    std::vector<ScoreEntry> scores;
    std::ifstream file(SCORE_FILE, std::ios::binary);
    
    if (!file) return scores;

    ScoreEntry entry;
    while (file.read((char*)&entry, sizeof(ScoreEntry))) {
        scores.push_back(entry);
    }
    
    file.close();

    // Immer sortieren, damit der höchste Score oben steht
    std::sort(scores.begin(), scores.end(), CompareScores);
    return scores;
}

// Gibt den höchsten Score zurück
int GetTopScore() {
    std::vector<ScoreEntry> scores = LoadScoreboard();
    if (scores.empty()) return 0;
    
    // Da LoadScoreboard bereits sortiert, ist der erste Eintrag der beste
    return scores[0].score;
}

// Fügt einen neuen Score hinzu und speichert die Top 10
void AddOrUpdateScore(const char* name, int score, float time) {
    std::vector<ScoreEntry> scores = LoadScoreboard();

    ScoreEntry newEntry;
    newEntry.score = score;
    newEntry.time = time; // FIX: Heißt 'time' laut Header, nicht 'timeSurvived'
    
    // Namen sicher kopieren
    if (name && std::strlen(name) > 0) {
        std::strncpy(newEntry.name, name, 15);
    } else {
        std::strncpy(newEntry.name, "Gast", 15);
    }
    newEntry.name[15] = '\0'; 

    scores.push_back(newEntry);
    std::sort(scores.begin(), scores.end(), CompareScores);

    // Nur die besten 10 behalten
    if (scores.size() > 10) scores.resize(10);

    std::ofstream outFile(SCORE_FILE, std::ios::binary | std::ios::trunc);
    if (outFile.is_open()) {
        for (const auto& e : scores) {
            outFile.write((const char*)&e, sizeof(ScoreEntry));
        }
        outFile.close();
    }
}

// Löscht die gesamte Bestenliste
void ClearScoreboard() {
    std::remove(SCORE_FILE); 
}