#pragma once
#include <vector>
#include <string>
#include <raylib.h>

// Forward declarations for block and enemy types
struct Block {
    Vector2 position;
    int type;
};
struct Enemy {
    Vector2 position;
    int type;
};

class GameManaging {
public:
    GameManaging();
    void LoadLevel(const std::string& filename);
    void DrawLevel();
    void DrawStats();
    void UpdateTime();
    void DecreaseLife();
    void ResetGame();

    int GetLives() const { return lives_; }
    int GetPoints() const { return points_; }
    float GetGameTime() const { return gameTime_; }

private:
    int lives_;
    float gameTime_;
    int points_;
    int countdownSeconds_;

    // Level data
    std::vector<Block> blocks_;
    std::vector<Enemy> enemies_;
    int backgroundType_;
};
