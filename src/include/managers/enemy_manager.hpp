#pragma once
#include "include/core/game_managing.hpp"
#include "include/enemies/enemy.hpp"
#include <memory>
#include <unordered_map>
#include <vector>

class Character;        // Forward declaration
class CollisionHandler; // Forward declaration
class ProjectilePool;   // Forward declaration
class GameManaging;     // Forward declaration
class EnemyManager {
private:
  std::vector<Enemy *> enemies;
  std::unordered_map<Enemy *, MovementStrategy *> pausedStrategies_;
  // Character references for simple player detection (Bowser fireball shooting
  // only)
  Vector2 *marioPosition_;
  Vector2 *luigiPosition_;
  Vector2 *activeCharacterPosition_;

  // Collision system reference
  CollisionHandler *collisionHandler_;

  // Projectile system for enemy shooting
  static ProjectilePool *projectilePool_;

  // Game state
  float difficultyMultiplier_;
  bool isPaused_;
  float globalSpeedMultiplier_;
  GameManaging *gameManager_ = nullptr;
  // Boss management
  Enemy *currentBoss_;

  // Singleton pattern implementation
  EnemyManager();
  EnemyManager(const EnemyManager &)            = delete;
  EnemyManager &operator=(const EnemyManager &) = delete;

public:
  // Singleton access method
  static EnemyManager &GetInstance();
  ~EnemyManager();

  // Core management functions (like ObjectManager)
  void SetCollisionHandler(CollisionHandler *handler);
  void AddEnemy(Enemy *enemy);
  void SpawnEnemy(EnemyType type, Vector2 position, int spriteId = 0);
  void SpawnBoss(Vector2 position, Character *character);

  void ClearAllEnemies();
  void ClearDeadEnemies();
  void SetGameManager(GameManaging *gameManager) {
    gameManager_ = gameManager;
  }
  void OnEnemyDeath(int index);

  // Update and rendering (like ObjectManager)
  void SetFrameCount();
  void UpdateAll(float deltaTime);
  void Draw();

  // Character reference for simple player detection (Bowser fireball shooting
  // only)
  void SetCharacterReferences(Vector2 *mario, Vector2 *luigi, Vector2 *active);

  // Projectile system
  static void SetProjectilePool(ProjectilePool *pool) {
    projectilePool_ = pool;
  }
  static ProjectilePool *GetProjectilePool() {
    return projectilePool_;
  }

  // Boss management

  Enemy *GetBoss() const {
    return currentBoss_;
  }
  bool HasBoss() const {
    return currentBoss_ != nullptr && currentBoss_->IsAlive();
  }

  // Basic utility functions
  size_t GetEnemyCount() const;
  size_t GetAliveEnemyCount() const;
  std::vector<Enemy *> GetEnemies() const {
    return enemies;
  }
  std::vector<Enemy *> GetEnemiesByType(EnemyType type) const;

  // Basic enemy state queries
  bool AreAllEnemiesDead() const;
  bool HasEnemyType(EnemyType type) const;

  // Simple pause/resume (stub methods for compatibility)
  void PauseAllEnemies() {
    isPaused_ = true;
  }
  void ResumeAllEnemies() {
    isPaused_ = false;
  }

  void RemoveEnemy(int index);

private:
  // Helper methods
  Enemy *CreateEnemyByType(EnemyType type, Vector2 position, int spriteId);
  void CleanupDeadEnemies();
};
