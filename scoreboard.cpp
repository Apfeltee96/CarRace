
#include "scoreboard.h"
#include <algorithm>
#include <fstream>
#include <cstring>
#include <filesystem>

static const char* SCORE_FILE = "scoreboard.dat";

// Absteigend nach Score sortieren
static bool CompareScores(const ScoreEntry& a, const ScoreEntry& b) {
    return a.score > b.score;
}

std::vector<ScoreEntry> LoadScoreboard() {
    std::vector<ScoreEntry> scores;

    std::ifstream file(SCORE_FILE, std::ios::binary);
    if (!file.is_open()) return scores;

    ScoreEntry entry;
    while (file.read(reinterpret_cast<char*>(&entry), sizeof(ScoreEntry))) {
        scores.push_back(entry);
    }

    std::sort(scores.begin(), scores.end(), CompareScores);
    return scores;
}

int GetTopScore() {
    std::vector<ScoreEntry> scores = LoadScoreboard();
    return scores.empty() ? 0 : scores[0].score;
}

void AddOrUpdateScore(const char* name, int score, float time) {
    std::vector<ScoreEntry> scores = LoadScoreboard();

    ScoreEntry entry;
    entry.score = score;
    entry.time  = time;

    const char* validName = (name && std::strlen(name) > 0) ? name : "Gast";
    std::strncpy(entry.name, validName, 15);
    entry.name[15] = '\0';

    scores.push_back(entry);
    std::sort(scores.begin(), scores.end(), CompareScores);
    if (scores.size() > 10) scores.resize(10);

    std::ofstream outFile(SCORE_FILE, std::ios::binary | std::ios::trunc);
    if (!outFile.is_open()) return;

    for (const auto& e : scores) {
        outFile.write(reinterpret_cast<const char*>(&e), sizeof(ScoreEntry));
    }
}

void ClearScoreboard() {
    std::filesystem::remove(SAVE_FILE);
}
