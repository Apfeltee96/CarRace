#include "scoreboard.h"
#include <fstream>
#include <algorithm> // Für std::sort

// Der Name der Datei, in der die Daten dauerhaft gespeichert werden
const std::string SCORE_FILE = "scoreboard.txt";

// Unser Sortier-Algorithmus: Wer das höchste Level hat, steht oben!
bool CompareScores(const ScoreEntry& a, const ScoreEntry& b) {
    return a.level > b.level;
}

std::vector<ScoreEntry> LoadScoreboard() {
    std::vector<ScoreEntry> scores;
    std::ifstream file(SCORE_FILE);
    
    if (file.is_open()) {
        std::string name;
        int level;
        // Lese Name und Level nacheinander aus der Textdatei
        while (file >> name >> level) {
            scores.push_back({name, level});
        }
        file.close();
    }
    
    // Nach dem Laden direkt sortieren
    std::sort(scores.begin(), scores.end(), CompareScores);
    return scores;
}

void SaveScoreboard(const std::vector<ScoreEntry>& scores) {
    std::ofstream file(SCORE_FILE);
    if (file.is_open()) {
        for (const auto& entry : scores) {
            // Speichere den Namen, ein Leerzeichen, das Level und einen Zeilenumbruch
            file << entry.name << " " << entry.level << "\n";
        }
        file.close();
    }
}

void AddOrUpdateScore(std::string name, int level) {
    std::vector<ScoreEntry> scores = LoadScoreboard();
    bool playerFound = false;
    
    // Prüfe, ob der Spieler schon in der Liste steht
    for (auto& entry : scores) {
        if (entry.name == name) {
            playerFound = true;
            // Nur updaten, wenn das neue Level HÖHER ist als das alte
            if (level > entry.level) {
                entry.level = level; 
            }
            break;
        }
    }
    
    // Wenn es ein neuer Spieler ist, füge ihn hinzu
    if (!playerFound) {
        scores.push_back({name, level});
    }
    
    // Liste neu sortieren
    std::sort(scores.begin(), scores.end(), CompareScores);
    
    // Optional: Schneide die Liste ab, damit nur die Top 10 gespeichert werden
    if (scores.size() > 10) {
        scores.resize(10);
    }
    
    SaveScoreboard(scores);
}