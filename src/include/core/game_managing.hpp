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
#include "include/collision/collision_handler.hpp"

class Character;
class Command;
class EnemyManager;

struct Particle {
  Vector2 position;
  Vector2 velocity;
  Color color;
  float life;
  float maxLife;
};

class GameManaging {
private:
  // Game state
  int lives_;
  float gameTime_;
  int points_;
  int countdownSeconds_;
  int backgroundType_;
  int currentLevel_;
  bool levelComplete_;
  bool gameOver_;
  Vector2 spawnPoint_;
  int updateCounter_;

  // Camera system
  Camera2D camera_;
  Vector2 cameraTarget_;
  float cameraOffsetX_;
  float cameraOffsetY_;

  // Level boundaries
  float levelWidth_;
  float levelHeight_;
  float groundLevel_;

  // Game objects and systems
  std::vector<std::unique_ptr<GameObject>> blocks_;
  std::vector<std::unique_ptr<Enemy>> enemies_;
  std::unique_ptr<EnemyManager> enemyManager_;
  std::vector<Particle> particles_;
  
  // Collision system - now properly integrated
  std::unique_ptr<CollisionHandler> collisionHandler_;

public:
  GameManaging();
  ~GameManaging();

  // Collision system management
  void InitializeCollisionSystem(float worldWidth, float worldHeight);
  void RegisterCharacterWithCollision(Character *character);
  void RegisterEnemyWithCollision(Enemy *enemy);
  void UpdateCollisionSystem();

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
  void DrawMiniMap() const;   // Mini-map display
  void DrawLevelInfo() const; // Level information

  // Game state management
  void UpdateTime();
  void DecreaseLife();
  void AddPoints(int points);
  void UpdateCamera(Character* character); // Camera following
  
  // Block interactions
  void HitBlock(GameObject *block, Character *character);
  void CreatePowerUpFromBlock(Vector2 position, int blockType);

  // Enhanced enemy management
  void SpawnEnemy(EnemyType type, Vector2 position);
  void SpawnBoss(Vector2 position);
  void SpawnEnemyFormation(); // Spawn test formation of enemies
  void SpawnEnemyWave(int waveNumber); // Wave-based spawning
  void RemoveDeadEnemies();
  
  // Level progression
  void LoadNextLevel(); // Level progression
  void CheckLevelComplete(); // Level completion check
  
  // Environmental systems
  void UpdatePhysics(float deltaTime); // Enhanced physics
  void UpdateParticles(float deltaTime); // Particle effects
  void HandleSpecialBlocks(Character* character); // Special block types
  
  // Advanced enemy control methods
  void SetEnemyDifficulty(float multiplier);
  void PauseEnemies();
  void ResumeEnemies();
  void ActivateBossRageMode();
  void HandleEnemyAI(float deltaTime); // Enhanced AI

  // Boss-specific methods
  bool IsBossDefeated() const;
  int GetBossHP() const;
  void SpawnBossEffects(); // Special effects

  // Level creation helpers
  void CreateLevel1();
  void CreateLevel2();
  void CreateLevel3();
  void CreateBossLevel();
  void SpawnLevelEnemies(int levelNumber);

  // Utility methods for level building
  void CreatePlatform(Vector2 start, Vector2 end, int blockType);
  void CreateStairs(Vector2 start, int steps, bool ascending);
  void CreatePipe(Vector2 position, int height);

  // Particle system
  void SpawnParticle(Vector2 position, Color color, Vector2 velocity = {0, 0});
  void CleanupDeadParticles();

  // Helper methods
  Rectangle GetBlockCollisionRect(const GameObject *block) const;
  bool IsOnGround(Character *character) const;
  void HandleVerticalCollision(Character *character, const GameObject *block);
  void HandleHorizontalCollision(Character *character, const GameObject *block);
  void PlaySoundEffect(const std::string &soundName);
  void UpdateBackgroundMusic();

  // Block and enemy creation
  void LoadResources();
  void CreateBlockFromType(int tileType, Vector2 position);
  void CreateEnemyFromType(int enemyType, Vector2 position);

  // Game state queries
  bool IsLevelComplete() const;
  bool IsGameOver() const;
  int GetLives() const { return lives_; }
  int GetPoints() const { return points_; }
  int GetCurrentLevel() const { return currentLevel_; }
  Camera2D GetCamera() const { return camera_; }
};
