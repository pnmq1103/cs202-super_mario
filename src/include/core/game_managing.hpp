#pragma once
#include <memory>
#include <raylib.h>
#include <string>
#include <vector>
#include <map>

// Include the actual GameObject and Enemy classes
#include "include/objects/game_object.hpp"
#include "include/enemies/enemy.hpp"
#include "include/collision/collision_handler.hpp"

class Character;

struct Particle {
  Vector2 position;
  Vector2 velocity;
  Color color;
  float life;
  float maxLife;
};

class GameManaging {
private:
  // Core game state
  int lives_;
  float gameTime_;
  int points_;
  int countdownSeconds_;
  bool gameOver_;
  bool levelComplete_;
  Vector2 spawnPoint_;

  // Level progression
  int currentLevel_;
  int maxLevels_;
  float levelEndX_;

  // Camera reference from Scene (read-only)
  Camera2D* sceneCamera_;

  // Level boundaries
  float levelWidth_;
  float levelHeight_;
  float groundLevel_;

  // Simple particle system for effects
  std::vector<Particle> particles_;
  
  // Collision system reference
  CollisionHandler* collisionHandler_;
  
  // Pause state
  bool isPaused_ = false;

  // Flag for handling death
  bool isDeathHandled_ = false;

public:
  GameManaging();
  ~GameManaging();

  // Essential setup methods
  void SetSceneCamera(Camera2D* camera);
  Camera2D GetCamera() const { return sceneCamera_ ? *sceneCamera_ : Camera2D{}; }
  void SetCollisionHandler(CollisionHandler* handler);
  void InitializeCollisionSystem(float worldWidth, float worldHeight);
  void RegisterCharacterWithCollision(Character* character);

  // Level management
  void LoadLevel(const std::string& filename);
  void UnloadLevel();
  void ResetGame();
  void LoadNextLevel();
  void CreateFallbackLevel();
  void ApplyLevelDifficulty();
  Vector2 GetSpawnPoint() const {
    return spawnPoint_;
  }

  // Core update loop
  void Update(float deltaTime, Character* activeCharacter);
  void UpdateCollisionSystem();
  void CheckLevelCompletion(Character* activeCharacter);
  void RestartCurrentLevel(Character *character);
  void OnPlayerDeath(Character *character);

  // Essential rendering
  void DrawLevel();
  void DrawStats() const;
  void DrawBackground();

  // Core game mechanics
  void UpdateTime();
  void AddPoints(int points);
  void DecreaseLife();
  void HitBlock(GameObject* block, Character* character);

  // Simple particle effects
  void SpawnParticle(Vector2 position, Color color, Vector2 velocity = {0, 0});
  void CleanupDeadParticles();

  // Object creation from map data
  void CreateEnemyFromType(int enemyType, Vector2 position);
  void CreateBlockFromType(int tileType, Vector2 position, Rectangle tileRect);
  void CreateSpecialObjectFromType(int specialType, Vector2 position);
  void ResumeEnemies();
  
  // Game state queries
  bool IsGameOver() const;
  bool IsLevelComplete() const;
  bool CanAdvanceLevel() const;
  int GetLives() const { return lives_; }
  int GetPoints() const { return points_; }
  int GetCurrentLevel() const;
  int GetMaxLevels() const;
  void SetLives(int lives);
  void SetCurrentLevel(int level);
  
  // Simple pause/transition state methods for game.cpp compatibility
  void TogglePause() { 
    isPaused_ = !isPaused_;
    if (isPaused_) {
      App.Media().TogglePauseMusic();
      App.Media().PlaySound("pause");
    } else {
      App.Media().TogglePauseMusic();
    }
  }
  bool IsPaused() const { return isPaused_; }
  bool IsInTransition() const { return false; /* Simple return false for now */ }

private:
  // Helper methods
  void UpdateParticles(float deltaTime);
  void PlaySoundEffect(const std::string& soundName);
  void UpdateBackgroundMusic();
};
