#ifndef SCOREBOARD_H
#define SCOREBOARD_H

#include <vector>

struct ScoreEntry {
    char name[16];
    int score;
    float time; // Wichtig: Muss exakt so heißen wie in der .cpp
};

// Diese Prototypen sagen allen anderen Dateien, dass es diese Funktionen gibt:
void AddOrUpdateScore(const char* name, int score, float time);
std::vector<ScoreEntry> LoadScoreboard();
void ClearScoreboard(); 
int GetTopScore();      

#endif