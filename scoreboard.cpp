#include "scoreboard.h"
#include <fstream>
#include <algorithm>

const std::string SCORE_FILE = "scoreboard.txt";

bool CompareScores(const ScoreEntry& a, const ScoreEntry& b) {
    return a.score > b.score; // Sortiert nach den meisten Punkten!
}

std::vector<ScoreEntry> LoadScoreboard() {
    std::vector<ScoreEntry> scores;
    std::ifstream file(SCORE_FILE);
    if (file.is_open()) {
        std::string name;
        int score;
        float time;
        while (file >> name >> score >> time) {
            scores.push_back({name, score, time});
        }
        file.close();
    }
    return scores;
}

void SaveScoreboard(const std::vector<ScoreEntry>& scores) {
    std::ofstream file(SCORE_FILE);
    if (file.is_open()) {
        for (const auto& entry : scores) {
            file << entry.name << " " << entry.score << " " << entry.timeSurvived << "\n";
        }
        file.close();
    }
}

void AddOrUpdateScore(const std::string& name, int score, float timeSurvived) {
    std::vector<ScoreEntry> scores = LoadScoreboard();
    bool found = false;
    for (auto& entry : scores) {
        if (entry.name == name) {
            if (score > entry.score) {
                entry.score = score;
                entry.timeSurvived = timeSurvived;
            }
            found = true;
            break;
        }
    }
    if (!found) {
        scores.push_back({name, score, timeSurvived});
    }
    std::sort(scores.begin(), scores.end(), CompareScores);
    if (scores.size() > 10) scores.resize(10);
    SaveScoreboard(scores);
}