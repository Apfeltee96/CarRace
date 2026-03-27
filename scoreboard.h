#ifndef SCOREBOARD_H
#define SCOREBOARD_H

#include <vector>
#include <string>

struct ScoreEntry {
    char name[16];     // Festgelegt auf 15 Zeichen + Null-Terminator
    int score;
    float timeSurvived;
};

std::vector<ScoreEntry> LoadScoreboard();
void AddOrUpdateScore(const char* name, int score, float time); 
void ClearScoreboard();

#endif