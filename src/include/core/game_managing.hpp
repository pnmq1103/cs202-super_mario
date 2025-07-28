#pragma once
#include <memory>
#include <raylib.h>
#include <string>
#include <unordered_map>
#include <vector>

// Include the actual GameObject and Enemy classes
#include "include/objects/game_object.hpp"
#include "include/objects/static_block.hpp"
#include "include/objects/question_block.hpp"
#include "include/objects/brick_block.hpp"
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
  void LoadLevel(const std::string &filename);
  void ResetGame();
  void UnloadLevel();

  // Core game update loop
  void Update(float deltaTime, Character *activeCharacter);

  // Rendering
  void DrawLevel();
  void DrawBackground();
  void DrawBlock(const GameObject *block);
  void DrawEnemy(const Enemy *enemy);
  void DrawEnemiesSimple();   // Fallback rendering
  void DrawEnemiesAdvanced(); // Enhanced enemy rendering with effects
  void DrawStats() const;
  void DrawMiniMap() const;   // New: Mini-map display
  void DrawLevelInfo() const; // New: Level information

  // Game state management
  void UpdateTime();
  void DecreaseLife();
  void AddPoints(int points);
  void UpdateCamera(Character* character); // New: Camera following
  
  // Enhanced collision detection
  void CheckSimpleCollisions(Character *character);
  void CheckAdvancedCollisions(Character *character); // New: Better collision
  void CheckPowerUpCollisions(Character *character);  // New: Power-up pickups
  void CheckEnemyCollisions(Character *character);    // New: Enemy interactions

  // Collision system initialization
  void InitializeCollisionSystem(Character *character);

  // Block interactions
  void HitBlock(GameObject *block, Character *character);
  void CreatePowerUpFromBlock(Vector2 position, int blockType); // New: Power-ups

  // Enhanced enemy management
  void SpawnEnemy(EnemyType type, Vector2 position);
  void SpawnBoss(Vector2 position);
  void SpawnEnemyFormation(); // Spawn test formation of enemies
  void SpawnEnemyWave(int waveNumber); // New: Wave-based spawning
  void RemoveDeadEnemies();
  
  // Level progression
  void LoadNextLevel(); // New: Level progression
  void CheckLevelComplete(); // New: Level completion check
  
  // Environmental systems
  void UpdatePhysics(float deltaTime); // New: Enhanced physics
  void UpdateParticles(float deltaTime); // New: Particle effects
  void HandleSpecialBlocks(Character* character); // New: Special block types
  
  // Advanced enemy control methods
  void SetEnemyDifficulty(float multiplier);
  void PauseEnemies();
  void ResumeEnemies();
  void ActivateBossRageMode();
  void HandleEnemyAI(float deltaTime); // New: Enhanced AI

  // Boss-specific methods
  bool IsBossDefeated() const;
  int GetBossHP() const;
  void SpawnBossEffects(); // New: Boss visual effects

  // World and level management
  void CreateLevel1(); // New: Predefined level layouts
  void CreateLevel2();
  void CreateLevel3();
  void CreateBossLevel();
  void GenerateTerrainBlocks(); // New: Terrain generation
  void SpawnLevelEnemies(int levelNumber); // New: Level-specific enemies
  
  // Game state queries
  bool IsLevelComplete() const;
  bool IsGameOver() const;
  int GetCurrentLevel() const { return currentLevel_; }
  Vector2 GetSpawnPoint() const { return spawnPoint_; }
  
  // Getters
  int GetLives() const {
    return lives_;
  }
  int GetPoints() const {
    return points_;
  }
  float GetGameTime() const {
    return gameTime_;
  }
  const std::vector<std::unique_ptr<GameObject>> &GetBlocks() const {
    return blocks_;
  }
  const std::vector<std::unique_ptr<Enemy>> &GetEnemies() const {
    return enemies_;
  }
  EnemyManager *GetEnemyManager() const {
    return enemyManager_.get();
  }
  Camera2D* GetCamera() { return &camera_; } // New: Camera access

private:
  // Game state
  int lives_;
  float gameTime_;
  int points_;
  int countdownSeconds_;
  int currentLevel_;
  bool levelComplete_;
  bool gameOver_;
  Vector2 spawnPoint_;

  // Level data
  std::vector<std::unique_ptr<GameObject>> blocks_;
  std::vector<std::unique_ptr<Enemy>> enemies_; // Legacy enemy system
  std::unique_ptr<EnemyManager> enemyManager_; // Enhanced strategy-based enemy management
  int backgroundType_;

  // Performance optimization
  int updateCounter_; // Counter for reducing update frequency

  // Camera system
  Camera2D camera_;
  Vector2 cameraTarget_;
  float cameraOffsetX_;
  float cameraOffsetY_;
  
  // Level boundaries
  float levelWidth_;
  float levelHeight_;
  float groundLevel_;
  
  // Visual effects
  struct Particle {
    Vector2 position;
    Vector2 velocity;
    Color color;
    float life;
    float maxLife;
  };
  std::vector<Particle> particles_;
  
  // Audio management
  void PlaySoundEffect(const std::string& soundName);
  void UpdateBackgroundMusic();

  // Helper methods
  void LoadResources();
  void CreateBlockFromType(int tileType, Vector2 position);
  void CreateEnemyFromType(int enemyType, Vector2 position);
  void UpdateParticleSystem(float deltaTime);
  void SpawnParticle(Vector2 position, Color color, Vector2 velocity = {0, 0});
  void CleanupDeadParticles();
  
  // Collision helpers
  Rectangle GetBlockCollisionRect(const GameObject* block) const;
  bool IsOnGround(Character* character) const;
  void HandleVerticalCollision(Character* character, const GameObject* block);
  void HandleHorizontalCollision(Character* character, const GameObject* block);
  
  // Level generation helpers
  void CreatePlatform(Vector2 start, Vector2 end, int blockType = 1);
  void CreateStairs(Vector2 start, int steps, bool ascending = true);
  void CreatePipe(Vector2 position, int height);
  void PlaceEnemyPattern(Vector2 startPos, const std::string& pattern);
};
