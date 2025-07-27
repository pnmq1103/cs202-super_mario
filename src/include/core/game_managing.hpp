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
class EnemyManager;
class CollisionHandler; // Forward declaration (temporarily unused)

class GameManaging {
public:
    GameManaging();
    ~GameManaging();
    
    // Level loading and management
    void LoadLevel(const std::string& filename);
    void ResetGame();
    void UnloadLevel();

    // Core game update loop
    void Update(float deltaTime, Character* activeCharacter);
    
    // Rendering
    void DrawLevel();
    void DrawBackground();
    void DrawBlock(const Block* block);
    void DrawEnemy(const Enemy* enemy);
    void DrawEnemiesSimple(); // Fallback rendering
    void DrawEnemiesAdvanced(); // Enhanced enemy rendering with effects
    void DrawStats() const;

    // Game state management
    void UpdateTime();
    void DecreaseLife();
    void AddPoints(int points);
    
    // ? Simple collision detection (replaces CollisionHandler temporarily)
    void CheckSimpleCollisions(Character* character);
    
    // Collision system initialization (temporarily disabled)
    void InitializeCollisionSystem(Character* character);
    
    // Block interactions
    void HitBlock(Block* block, Character* character);
    
    // ? Enhanced enemy management
    void SpawnEnemy(EnemyType type, Vector2 position);
    void SpawnBoss(Vector2 position);
    void SpawnEnemyFormation(); // Spawn test formation of enemies
    void RemoveDeadEnemies();
    
    // ? Advanced enemy control methods
    void SetEnemyDifficulty(float multiplier);
    void PauseEnemies();
    void ResumeEnemies();
    void ActivateBossRageMode();
    
    // Boss-specific methods
    bool IsBossDefeated() const;
    int GetBossHP() const;
    
    // Getters
    int GetLives() const { return lives_; }
    int GetPoints() const { return points_; }
    float GetGameTime() const { return gameTime_; }
    const std::vector<std::unique_ptr<Block>>& GetBlocks() const { return blocks_; }
    const std::vector<std::unique_ptr<Enemy>>& GetEnemies() const { return enemies_; }
    EnemyManager* GetEnemyManager() const { return enemyManager_.get(); }
    // CollisionHandler* GetCollisionHandler() const { return collisionHandler_.get(); } // Temporarily disabled

private:
    // Game state
    int lives_;
    float gameTime_;
    int points_;
    int countdownSeconds_;
    
    // Level data
    std::vector<std::unique_ptr<Block>> blocks_;
    std::vector<std::unique_ptr<Enemy>> enemies_; // Legacy enemy system
    std::unique_ptr<EnemyManager> enemyManager_; // ? Enhanced strategy-based enemy management
    int backgroundType_;
    
    // Performance optimization
    int updateCounter_; // Counter for reducing update frequency
    
    // ? Collision system - temporarily disabled to fix input issues
    // std::unique_ptr<CollisionHandler> collisionHandler_;
    
    // Helper methods
    void LoadResources();
    void CreateBlockFromType(int tileType, Vector2 position);
    void CreateEnemyFromType(int enemyType, Vector2 position);
};
