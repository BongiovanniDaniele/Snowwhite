#include "DataClasses/HighScoreManager.h" // Assicurati che il percorso corrisponda alle tue cartelle

std::vector<ScoreEntry> HighScoreManager::loadScores(const std::string& filename) {
    std::vector<ScoreEntry> scores;
    std::ifstream file(filename);
    std::string name;
    int score;

    if (file.is_open()) {
        while (file >> name >> score) {
            scores.push_back({ name, score });
        }
        file.close();
    }
    // Assicuriamoci che siano sempre ordinati
    std::sort(scores.begin(), scores.end(), std::greater<ScoreEntry>());
    return scores;
}

void HighScoreManager::saveScores(const std::string& filename, std::vector<ScoreEntry>& scores) {
    std::ofstream file(filename);
    if (file.is_open()) {
        for (const auto& entry : scores) {
            file << entry.name << " " << entry.score << "\n";
        }
        file.close();
    }
}

bool HighScoreManager::isNewHighScore(int newScore, const std::string& filename) {
    auto scores = loadScores(filename);
    if (scores.size() < 10) return true; // C'è spazio vuoto
    return newScore > scores.back().score; // È maggiore dell'ultimo
}

void HighScoreManager::addScore(std::string name, int score, const std::string& filename) {
    auto scores = loadScores(filename);
    scores.push_back({ name, score });

    // Ordina dal più alto al più basso
    std::sort(scores.begin(), scores.end(), std::greater<ScoreEntry>());

    // Mantieni solo i primi 10
    if (scores.size() > 10) {
        scores.resize(10);
    }

    saveScores(filename, scores);
}