#ifndef SCOREBOARD_H
#define SCOREBOARD_H

#include <vector>
#include <string>

// Ein einzelner Eintrag in der Bestenliste.
struct ScoreEntry
{
    std::string name;
    int score;
    float time;
};

std::vector<ScoreEntry> LoadScoreboard();
void AddOrUpdateScore(const std::string &name, int score, float time);
void ClearScoreboard();
int GetTopScore();

#endif
