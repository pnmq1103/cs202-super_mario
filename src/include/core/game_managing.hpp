#pragma once
#include <vector>
#include <string>
#include <memory>
#include <unordered_map>
#include <raylib.h>

// Include the actual Block and Enemy classes
#include "include/blocks/block.hpp"
#include "include/enemies/enemy.hpp"

class Character;
class Command;

class GameManaging {
public:
    GameManaging();
    ~GameManaging();
    
    // Level loading and management
    void LoadLevel(const std::string& path);
    void ResetGame();
    void UnloadLevel();

    // Core game update loop
    void Update(float deltaTime, Character* activeCharacter);
    
    // Rendering
    void DrawLevel();
    void DrawBackground();
    void DrawBlock(const Block* block);
    void DrawEnemy(const Enemy* enemy);
    void DrawStats() const;

    // Game state management
    void UpdateTime();
    void DecreaseLife();
    void AddPoints(int points);
    
    // Collision detection
    void CheckCollisions(Character* character);
    bool CheckBlockCollision(Rectangle characterRect, Vector2& correction);
    void CheckEnemyCollisions(Character* character);
    
    // Block interactions
    void HitBlock(Block* block, Character* character);
    
    // Enemy management
    void SpawnEnemy(EnemyType type, Vector2 position);
    void RemoveDeadEnemies();
    
    // Getters
    int GetLives() const { return lives_; }
    int GetPoints() const { return points_; }
    float GetGameTime() const { return gameTime_; }
    const std::vector<std::unique_ptr<Block>>& GetBlocks() const { return blocks_; }
    const std::vector<std::unique_ptr<Enemy>>& GetEnemies() const { return enemies_; }

private:
    // Game state
    int lives_;
    float gameTime_;
    int points_;
    int countdownSeconds_;
    
    // Level data
    std::vector<std::unique_ptr<Block>> blocks_;
    std::vector<std::unique_ptr<Enemy>> enemies_;
    Texture background_;
    int backgroundType_;
    
    // Helper methods
    void LoadResources();
    void CreateBlockFromType(BlockType type, int gid, Vector2 pos, Vector2 size, int spriteID, bool solid, bool animating);
    void CreateEnemyFromType(EnemyType type, Vector2 pos, Vector2 size, Vector2 velo, int spriteID);
};
