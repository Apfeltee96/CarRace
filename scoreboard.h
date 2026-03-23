#ifndef SCOREBOARD_H
#define SCOREBOARD_H
#include <vector>
#include <string>

struct ScoreEntry {
    std::string name;
    int score; // NEU: Punkte statt Level!
    float timeSurvived;
};

void AddOrUpdateScore(const std::string& name, int score, float timeSurvived);
std::vector<ScoreEntry> LoadScoreboard();
void SaveScoreboard(const std::vector<ScoreEntry>& scores);

#endif