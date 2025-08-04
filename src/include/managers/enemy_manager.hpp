#pragma once
#include "include/enemies/enemy.hpp"
#include <memory>
#include <unordered_map>
#include <vector>

class Character;        // Forward declaration
class CollisionHandler; // Forward declaration
class ProjectilePool;   // Forward declaration

class EnemyManager {
private:
  std::vector<Enemy *> enemies;
  std::unordered_map<Enemy *, MovementStrategy *> pausedStrategies_;

  // Character references for AI only (not collision)
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

  void ClearAllEnemies();
  void ClearDeadEnemies();

  // Update and rendering (like ObjectManager)
  void SetFrameCount();
  void UpdateAll(float deltaTime);
  void Draw();

  // Character reference for AI (not collision)
  void SetCharacterReferences(Vector2 *mario, Vector2 *luigi, Vector2 *active);

  // Projectile system
  static void SetProjectilePool(ProjectilePool *pool) {
    projectilePool_ = pool;
  }
  static ProjectilePool *GetProjectilePool() {
    return projectilePool_;
  }

  // AI and behavior control (non-collision)
  void UpdateEnemyBehaviors(float deltaTime);
  void UpdateEnemyAI(float deltaTime);
  void UpdateBossAI(float deltaTime);

  // Game state management
  void SetDifficulty(float difficultyMultiplier);
  void SetGlobalSpeedMultiplier(float multiplier);
  void PauseAllEnemies();
  void ResumeAllEnemies();

  // Boss management
  void SpawnBoss(EnemyType bossType, Vector2 position);
  Enemy *GetBoss() const {
    return currentBoss_;
  }
  bool HasBoss() const {
    return currentBoss_ != nullptr && currentBoss_->IsAlive();
  }
  void SetBossRageMode();

  // Enemy spawning utilities
  void SpawnFormation(
    EnemyType type, Vector2 startPos, int count, float spacing = 50.0f);
  void SpawnWave(const std::vector<std::pair<EnemyType, Vector2>> &wave);
  void SpawnPatrolGroup(
    EnemyType type, Vector2 pointA, Vector2 pointB, int count = 2);

  // AI behavior modification
  void EnableAggressiveMode(bool aggressive = true);
  void SetEnemyTarget(Enemy *enemy, Vector2 *target);
  void MakeEnemyFollow(Enemy *enemy, Vector2 *target, float speed = 60.0f);
  void MakeEnemyPatrol(
    Enemy *enemy, Vector2 pointA, Vector2 pointB, float speed = 40.0f);
  void MakeEnemyJump(Enemy *enemy, float interval = 2.0f, float force = 200.0f);

  // Utility and query functions
  size_t GetEnemyCount() const;
  size_t GetAliveEnemyCount() const;
  std::vector<Enemy *> GetEnemies() const {
    return enemies;
  }
  std::vector<Enemy *> GetEnemiesInRadius(Vector2 center, float radius) const;
  std::vector<Enemy *> GetEnemiesByType(EnemyType type) const;
  std::vector<Enemy *> GetEnemiesInState(EnemyState state) const;

  // Enemy state queries
  bool AreAllEnemiesDead() const;
  bool HasEnemyType(EnemyType type) const;
  Enemy *GetNearestEnemyToPosition(Vector2 position) const;
  Enemy *GetFarthestEnemyFromPosition(Vector2 position) const;

  // Special effects for gameplay
  void DamageEnemiesInRadius(Vector2 center, float radius, int damage = 1);
  void
  KnockbackEnemiesInRadius(Vector2 center, float radius, float force = 100.0f);
  void
  StunAllEnemiesInRadius(Vector2 center, float radius, float duration = 2.0f);
  void HandleProjectileCollisions(
    Vector2 projectilePos, float projectileRadius, int damage = 1);

  void ApplyStunEffect(Vector2 center, float radius, float duration = 2.0f);
  void ApplyKnockbackEffect(Vector2 center, float radius, float force = 100.0f);
  void ApplySlowEffect(float duration = 3.0f, float speedMultiplier = 0.5f);
  void ApplyFreezeEffect(float duration = 2.0f);

  // Enemy behavior modification
  void ModifyEnemySpeed(Enemy *enemy, float speedMultiplier);
  void ModifyEnemyAggression(Enemy *enemy, bool aggressive);
  void ModifyEnemyDetectionRange(Enemy *enemy, float newRange);

  // Debug and development
  void DebugDrawEnemyInfo() const;
  void PrintEnemyStates() const;

  void RemoveEnemy(int index);

private:
  // Helper methods
  Enemy *CreateEnemyByType(EnemyType type, Vector2 position, int spriteId);
  void CleanupDeadEnemies();
  void UpdateEnemyTargeting(Enemy *enemy);
  void UpdateEnemyFormations(float deltaTime);
};
