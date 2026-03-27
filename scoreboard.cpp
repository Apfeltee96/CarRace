#define _CRT_SECURE_NO_WARNINGS
#include "scoreboard.h"
#include <algorithm>
#include <fstream>
#include <cstring>

const char* SCORE_FILE = "scoreboard.dat";

bool CompareScores(const ScoreEntry& a, const ScoreEntry& b) {
    return a.score > b.score; 
}

std::vector<ScoreEntry> LoadScoreboard() {
    std::vector<ScoreEntry> scores;
    std::ifstream file(SCORE_FILE, std::ios::binary);
    
    if (!file) return scores;

    ScoreEntry entry;
    // Wir lesen einfach blockweise, solange die Datei Daten hat
    while (file.read((char*)&entry, sizeof(ScoreEntry))) {
        scores.push_back(entry);
    }
    
    file.close();
    return scores;
}

void AddOrUpdateScore(const char* name, int score, float time) {
    std::vector<ScoreEntry> scores = LoadScoreboard();

    ScoreEntry newEntry;
    newEntry.score = score;
    newEntry.timeSurvived = time;
    
    // Namen sicher kopieren
    std::strncpy(newEntry.name, (name && strlen(name) > 0) ? name : "Gast", 15);
    newEntry.name[15] = '\0'; // Ende erzwingen

    scores.push_back(newEntry);
    std::sort(scores.begin(), scores.end(), CompareScores);

    if (scores.size() > 10) scores.resize(10);

    std::ofstream outFile(SCORE_FILE, std::ios::binary | std::ios::trunc);
    if (outFile.is_open()) {
        for (const auto& e : scores) {
            outFile.write((char*)&e, sizeof(ScoreEntry));
        }
        outFile.close();
    }
}

void ClearScoreboard() {
    std::remove(SCORE_FILE); 
}