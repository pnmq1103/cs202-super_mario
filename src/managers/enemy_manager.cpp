#include "include/managers/enemy_manager.hpp"
#include "include/characters/character.hpp"
#include "include/characters/projectile_pool.hpp"
#include "include/collision/collision_handler.hpp"
#include "include/enemies/bowser.hpp"
#include "include/enemies/goomba.hpp"
#include "include/enemies/koopa_troopa.hpp"
#include "include/enemies/movement_strategy.hpp"
#include "include/enemies/piranha_plant.hpp"
#include <algorithm>
#include <cmath>
#include <iostream>

// Initialize static member
ProjectilePool *EnemyManager::projectilePool_ = nullptr;

EnemyManager::EnemyManager()
    : marioPosition_(nullptr), luigiPosition_(nullptr),
      activeCharacterPosition_(nullptr), collisionHandler_(nullptr),
      difficultyMultiplier_(1.0f), isPaused_(false),
      globalSpeedMultiplier_(1.0f), currentBoss_(nullptr) {}

// Singleton implementation
EnemyManager &EnemyManager::GetInstance() {
  static EnemyManager instance;
  return instance;
}

EnemyManager::~EnemyManager() {
  ClearAllEnemies();
}

// NOTE: SetProjectilePool and GetProjectilePool are implemented inline in the
// header

void EnemyManager::AddEnemy(Enemy *enemy) {
  if (enemy) {
    enemies.push_back(enemy);

    // Set character reference for simple player detection (for Bowser fireball
    // shooting)
    if (activeCharacterPosition_) {
      enemy->SetPlayerReference(activeCharacterPosition_);
    }

    // If this is a boss, track it
    if (enemy->GetType() == EnemyType::Bowser) {
      currentBoss_ = enemy;
    }

    // Add enemy to collision system if available
    if (collisionHandler_) {
      collisionHandler_->AddEnemy(enemy);
    }
  }
}

void EnemyManager::SpawnEnemy(EnemyType type, Vector2 position, int spriteId) {
  Enemy *newEnemy = CreateEnemyByType(type, position, spriteId);
  if (newEnemy) {
    AddEnemy(newEnemy);
  }
}

Enemy *EnemyManager::CreateEnemyByType(
  EnemyType type, Vector2 position, int spriteId) {
  Enemy *enemy = nullptr;
  float scale  = 4.0f; // Default scale similar to GameObject

  switch (type) {
    case EnemyType::Goomba:
      enemy = new Goomba(position, scale);
      break;
    case EnemyType::Koopa:
      enemy = new KoopaTroopa(position, scale);
      break;
    case EnemyType::Piranha:
      enemy = new PiranhaPlant(position, scale);
      break;
    case EnemyType::Bowser:
      enemy = new Bowser(position, scale);
      break;
    default:
      break;
  }

  return enemy;
}

void EnemyManager::SetCollisionHandler(CollisionHandler *collisionHandler) {
  collisionHandler_ = collisionHandler;

  // Add all existing enemies to collision system
  for (Enemy *enemy : enemies) {
    if (enemy && collisionHandler_) {
      collisionHandler_->AddEnemy(enemy);
    }
  }
}

void EnemyManager::ClearAllEnemies() {
  for (Enemy *enemy : enemies) {
    delete enemy;
  }
  enemies.clear();
  currentBoss_ = nullptr;
  pausedStrategies_.clear();
}

void EnemyManager::ClearDeadEnemies() {
  CleanupDeadEnemies();
}

void EnemyManager::CleanupDeadEnemies() {
  for (int i = 0; i < enemies.size(); ++i) {
    if (enemies[i] && (!enemies[i]->IsAlive() && enemies[i]->isPendingDeletion())) {
      if (enemies[i] == currentBoss_) {
        currentBoss_ = nullptr;
      }
      if (collisionHandler_) {
        collisionHandler_->RemoveEnemy(i);
      }
      delete enemies[i];
      enemies[i] = nullptr;
    }
  }
}
void EnemyManager::OnEnemyDeath(int index) {
  // Don't delete immediately, just mark for cleanup
  // The enemy will be removed during next CleanupDeadEnemies call
  if (index >= 0 && index < enemies.size() && enemies[index]) {
    // Make sure the enemy is marked for deletion
      if (gameManager_) {
        int point = 100;
        switch (enemies[index]->GetType()) {
          case EnemyType::Goomba:
            point = 100;
            break;
          case EnemyType::Koopa:
            point = 200;
            break;
          case EnemyType::Piranha:
            point = 300;
            break;
          case EnemyType::Bowser:
            point = 500; // Boss gives more points
            break;
          default:
            point = 100; // Default for unknown types
        }
        gameManager_->AddPoints(point);
      }
    enemies[index]->MarkForDeletion();
  }
}

void EnemyManager::UpdateAll(float deltaTime) {
  if (isPaused_)
    return;

  // Simple update - just call Update() on each enemy
  for (Enemy *enemy : enemies) {
    if (enemy && enemy->IsAlive()) {
      enemy->Update(); // Each enemy handles its own simple behavior
    }
  }

  // Clean up dead enemies
  CleanupDeadEnemies();
}

void EnemyManager::SetCharacterReferences(
  Vector2 *mario, Vector2 *luigi, Vector2 *active) {
  marioPosition_           = mario;
  luigiPosition_           = luigi;
  activeCharacterPosition_ = active;

  // Update all existing enemies with the new character reference
  // (Only needed for Bowser fireball shooting)
  for (Enemy *enemy : enemies) {
    if (enemy) {
      enemy->SetPlayerReference(activeCharacterPosition_);
    }
  }
}

// Keep SetFrameCount for consistency with GameObject pattern
void EnemyManager::SetFrameCount() {
  Enemy::SetFrameCount(); // Call static method like ObjectManager does
}

void EnemyManager::SpawnBoss(EnemyType bossType, Vector2 position) {
  if (bossType == EnemyType::Bowser) {
    SpawnEnemy(bossType, position, 0);
  }
}

// Utility functions
size_t EnemyManager::GetEnemyCount() const {
  return enemies.size();
}

size_t EnemyManager::GetAliveEnemyCount() const {
  return std::count_if(enemies.begin(), enemies.end(), [](const Enemy *enemy) {
    return enemy && enemy->IsAlive();
  });
}

std::vector<Enemy *> EnemyManager::GetEnemiesByType(EnemyType type) const {
  std::vector<Enemy *> typeEnemies;

  for (Enemy *enemy : enemies) {
    if (enemy && enemy->IsAlive() && enemy->GetType() == type) {
      typeEnemies.push_back(enemy);
    }
  }

  return typeEnemies;
}

bool EnemyManager::AreAllEnemiesDead() const {
  return GetAliveEnemyCount() == 0;
}

bool EnemyManager::HasEnemyType(EnemyType type) const {
  return !GetEnemiesByType(type).empty();
}

void EnemyManager::RemoveEnemy(int index) {
  if (index >= 0 && index < enemies.size()) {
    if (enemies[index]) {
      delete enemies[index];
      enemies[index] = nullptr;
      collisionHandler_->RemoveEnemy(index);
    }
  }
}

void EnemyManager::Draw() {
  for (int i = 0; i < enemies.size(); ++i) {
    if (enemies[i] && enemies[i]->IsAlive()) {
      enemies[i]->Draw();
    }
  }
}