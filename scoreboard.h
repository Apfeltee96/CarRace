#ifndef SCOREBOARD_H
#define SCOREBOARD_H

#include <vector>

// Ein einzelner Eintrag in der Bestenliste.
struct ScoreEntry
{
    char name[16];
    int score;
    float time;
};

std::vector<ScoreEntry> LoadScoreboard();
void AddOrUpdateScore(const char *name, int score, float time);
void ClearScoreboard();
int GetTopScore();

#endif
