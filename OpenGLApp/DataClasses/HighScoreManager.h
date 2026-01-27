#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <iostream>

struct ScoreEntry {
    std::string name;
    int score;

    // Order
    bool operator>(const ScoreEntry& other) const {
        return score > other.score;
    }
};

class HighScoreManager {
public:
    static std::vector<ScoreEntry> loadScores(const std::string& filename);
    static void saveScores(const std::string& filename, std::vector<ScoreEntry>& scores);
    static bool isNewHighScore(int newScore, const std::string& filename);
    static void addScore(std::string name, int score, const std::string& filename);
};