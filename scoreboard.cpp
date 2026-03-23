#include "scoreboard.h"
#include <algorithm>
#include <fstream>
#include <vector>

// Wir nutzen nur noch die .dat Datei für alles!
const char* SCORE_FILE = "scoreboard.dat";

bool CompareScores(const ScoreEntry& a, const ScoreEntry& b) {
    return a.score > b.score; 
}

std::vector<ScoreEntry> LoadScoreboard() {
    std::vector<ScoreEntry> scores;
    std::ifstream file(SCORE_FILE, std::ios::binary);
    
    if (file.is_open()) {
        int count = 0;
        if (file.read((char*)&count, sizeof(int))) {
            for (int i = 0; i < count; i++) {
                int nameLen = 0;
                file.read((char*)&nameLen, sizeof(int));
                
                char* nameBuf = new char[nameLen + 1];
                file.read(nameBuf, nameLen);
                nameBuf[nameLen] = '\0';
                
                int score = 0;
                float time = 0.0f;
                file.read((char*)&score, sizeof(int));
                file.read((char*)&time, sizeof(float));
                
                scores.push_back({std::string(nameBuf), score, time});
                delete[] nameBuf;
            }
        }
        file.close();
    }
    return scores;
}

// Hilfsfunktion zum Speichern (wird von AddOrUpdateScore genutzt)
void SaveScoreboard(const std::vector<ScoreEntry>& scores) {
    std::ofstream outFile(SCORE_FILE, std::ios::binary);
    if (outFile.is_open()) {
        int count = (int)scores.size();
        outFile.write((char*)&count, sizeof(int));
        for (const auto& entry : scores) {
            int nameLen = (int)entry.name.length();
            outFile.write((char*)&nameLen, sizeof(int));
            outFile.write(entry.name.c_str(), nameLen);
            outFile.write((char*)&entry.score, sizeof(int));
            outFile.write((char*)&entry.timeSurvived, sizeof(float));
        }
        outFile.close();
    }
}

void AddOrUpdateScore(const std::string& name, int score, float time) {
    // 1. Aktuelle Liste laden
    std::vector<ScoreEntry> scores = LoadScoreboard();

    // 2. Jedes Spiel wird ein neuer Eintrag (auch bei gleichem Namen)
    ScoreEntry newEntry;
    newEntry.name = name;
    newEntry.score = score;
    newEntry.timeSurvived = time;
    scores.push_back(newEntry);

    // 3. Sortieren: Höchste Punktzahl zuerst
    std::sort(scores.begin(), scores.end(), CompareScores);

    // 4. Nur die Top 10 behalten
    if (scores.size() > 10) {
        scores.resize(10);
    }

    // 5. In die Binärdatei schreiben
    SaveScoreboard(scores);
}