#include "scoreboard.h"
#include <fstream>
#include <algorithm>

const std::string SCORE_FILE = "scoreboard.txt";

// VERBESSERTER ALGORITHMUS: Sortiert nach Level, bei Gleichstand nach Zeit!
bool CompareScores(const ScoreEntry& a, const ScoreEntry& b) {
    if (a.level != b.level) {
        return a.level > b.level; 
    }
    return a.timeSurvived > b.timeSurvived; 
}

std::vector<ScoreEntry> LoadScoreboard() {
    std::vector<ScoreEntry> scores;
    std::ifstream file(SCORE_FILE);
    
    if (file.is_open()) {
        std::string name;
        int level;
        float time;
        // Lese Name, Level UND Zeit
        while (file >> name >> level >> time) {
            scores.push_back({name, level, time});
        }
        file.close();
    }
    
    std::sort(scores.begin(), scores.end(), CompareScores);
    return scores;
}

void SaveScoreboard(const std::vector<ScoreEntry>& scores) {
    std::ofstream file(SCORE_FILE);
    if (file.is_open()) {
        for (const auto& entry : scores) {
            file << entry.name << " " << entry.level << " " << entry.timeSurvived << "\n";
        }
        file.close();
    }
}

void AddOrUpdateScore(std::string name, int level, float timeSurvived) {
    std::vector<ScoreEntry> scores = LoadScoreboard();
    bool playerFound = false;
    
    for (auto& entry : scores) {
        if (entry.name == name) {
            playerFound = true;
            // Updaten, wenn Level HÖHER ist ODER (Level gleich UND Zeit besser ist)
            if (level > entry.level || (level == entry.level && timeSurvived > entry.timeSurvived)) {
                entry.level = level; 
                entry.timeSurvived = timeSurvived;
            }
            break;
        }
    }
    
    if (!playerFound) {
        scores.push_back({name, level, timeSurvived});
    }
    
    std::sort(scores.begin(), scores.end(), CompareScores);
    
    if (scores.size() > 10) {
        scores.resize(10);
    }
    
    SaveScoreboard(scores);
}