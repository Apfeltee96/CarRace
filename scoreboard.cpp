#include "scoreboard.h"
#include <algorithm>
#include <fstream>
#include <filesystem>

static const std::string SCORE_FILE = "scoreboard.dat";

static void writeString(std::ofstream &f, const std::string &s)
{
    int len = static_cast<int>(s.size());
    f.write(reinterpret_cast<const char *>(&len), sizeof(int));
    f.write(s.c_str(), len);
}

static std::string readString(std::ifstream &f)
{
    int len = 0;
    if (!f.read(reinterpret_cast<char *>(&len), sizeof(int)) || len < 0 || len > 100)
        return {};
    std::string s(len, '\0');
    f.read(s.data(), len);
    return s;
}

std::vector<ScoreEntry> LoadScoreboard()
{
    std::vector<ScoreEntry> scores;

    std::ifstream file(SCORE_FILE, std::ios::binary);
    if (!file.is_open())
        return scores;

    while (true)
    {
        ScoreEntry entry;
        entry.name = readString(file);
        if (file.fail())
            break;
        if (!file.read(reinterpret_cast<char *>(&entry.score), sizeof(int)))
            break;
        if (!file.read(reinterpret_cast<char *>(&entry.time), sizeof(float)))
            break;
        scores.push_back(std::move(entry));
    }

    std::sort(scores.begin(), scores.end(),
              [](const ScoreEntry &a, const ScoreEntry &b)
              { return a.score > b.score; });
    return scores;
}

int GetTopScore()
{
    auto scores = LoadScoreboard();
    return scores.empty() ? 0 : scores[0].score;
}

void AddOrUpdateScore(const std::string &name, int score, float time)
{
    auto scores = LoadScoreboard();

    ScoreEntry entry;
    entry.name = name.empty() ? "Gast" : name.substr(0, 15);
    entry.score = score;
    entry.time = time;

    scores.push_back(std::move(entry));
    std::sort(scores.begin(), scores.end(),
              [](const ScoreEntry &a, const ScoreEntry &b)
              { return a.score > b.score; });
    if (scores.size() > 10)
        scores.resize(10);

    std::ofstream outFile(SCORE_FILE, std::ios::binary | std::ios::trunc);
    if (!outFile.is_open())
        return;

    for (const auto &e : scores)
    {
        writeString(outFile, e.name);
        outFile.write(reinterpret_cast<const char *>(&e.score), sizeof(int));
        outFile.write(reinterpret_cast<const char *>(&e.time), sizeof(float));
    }
}

void ClearScoreboard()
{
    std::filesystem::remove(SCORE_FILE);
}
