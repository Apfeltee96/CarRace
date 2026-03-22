#pragma once
#include <string>
#include <vector>

struct ScoreEntry {
    std::string name;
    int level;
    float timeSurvived; // NEU: Die überlebte Zeit in diesem Level
};

std::vector<ScoreEntry> LoadScoreboard();
void SaveScoreboard(const std::vector<ScoreEntry>& scores);

// NEU: Die Funktion nimmt jetzt auch die Zeit entgegen
void AddOrUpdateScore(std::string name, int level, float timeSurvived);