#include "include/managers/enemy_manager.hpp"
#include "include/enemies/goomba.hpp"
#include "include/enemies/koopa_troopa.hpp"
#include "include/enemies/piranha_plant.hpp"
#include "include/enemies/bowser.hpp"
#include "include/enemies/movement_strategy.hpp"
#include "include/characters/character.hpp"
#include "include/collision/collision_handler.hpp"
#include "include/characters/projectile_pool.hpp"
#include <algorithm>
#include <cmath>
#include <iostream>

// Initialize static member
ProjectilePool* EnemyManager::projectilePool_ = nullptr;

EnemyManager::EnemyManager() 
    : marioPosition_(nullptr), luigiPosition_(nullptr), activeCharacterPosition_(nullptr),
      collisionHandler_(nullptr), difficultyMultiplier_(1.0f), isPaused_(false), globalSpeedMultiplier_(1.0f),
      currentBoss_(nullptr) {}

// Singleton implementation
EnemyManager& EnemyManager::GetInstance() {
    static EnemyManager instance;
    return instance;
}

EnemyManager::~EnemyManager() {
    ClearAllEnemies();
}

void EnemyManager::AddEnemy(Enemy* enemy) {
    if (enemy) {
        enemies.push_back(enemy);
        
        // Set character reference for player detection
        if (activeCharacterPosition_) {
            enemy->SetPlayerReference(activeCharacterPosition_);
        }
        
        // If this is a boss, track it
        if (enemy->GetType() == EnemyType::Bowser) {
            currentBoss_ = enemy;
        }
        
        // IMPORTANT: Add enemy to collision system if available
        if (collisionHandler_) {
            collisionHandler_->AddEnemy(enemy);
        }
    }
}

void EnemyManager::SpawnEnemy(EnemyType type, Vector2 position, int spriteId) {
    Enemy* newEnemy = CreateEnemyByType(type, position, spriteId);
    if (newEnemy) {
        AddEnemy(newEnemy);
    }
}

Enemy* EnemyManager::CreateEnemyByType(EnemyType type, Vector2 position, int spriteId) {
    Enemy* enemy = nullptr;
    float scale = 4.0f; // Default scale similar to GameObject
    
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

void EnemyManager::RemoveEnemy(Enemy* enemy) {
    if (!enemy) return;
    
    auto it = std::find(enemies.begin(), enemies.end(), enemy);
    if (it != enemies.end()) {
        if (*it == currentBoss_) {
            currentBoss_ = nullptr;
        }
        delete *it;
        enemies.erase(it);
    }
}

// Add method to set collision handler like ObjectManager::Reset()
void EnemyManager::SetCollisionHandler(CollisionHandler* collisionHandler) {
    collisionHandler_ = collisionHandler;
    
    // Add all existing enemies to collision system
    for (Enemy* enemy : enemies) {
        if (enemy && collisionHandler_) {
            collisionHandler_->AddEnemy(enemy);
        }
    }
}

void EnemyManager::ClearAllEnemies() {
    for (Enemy* enemy : enemies) {
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
    auto it = enemies.begin();
    while (it != enemies.end()) {
        if (!(*it)->IsAlive()) {
            if (*it == currentBoss_) {
                currentBoss_ = nullptr;
            }
            delete *it;
            it = enemies.erase(it);
        } else {
            ++it;
        }
    }
}

void EnemyManager::UpdateAll(float deltaTime) {
    if (isPaused_) return;
    
    // Apply global speed multiplier - not used directly but could affect enemy behavior
    (void)deltaTime; // This parameter isn't needed for the new GameObject-style enemies
    
    // Update each enemy - new GameObject style doesn't take deltaTime parameter
    for (Enemy* enemy : enemies) {
        if (enemy && enemy->IsAlive()) {
            enemy->Update(); // Remove deltaTime parameter
        }
    }
    
    // Update AI behaviors (non-collision related)
    UpdateEnemyAI(GetFrameTime());
    UpdateEnemyBehaviors(GetFrameTime());
    
    // Update boss AI (non-collision related)
    if (HasBoss()) {
        UpdateBossAI(GetFrameTime());
    }
    
    // Clean up dead enemies
    CleanupDeadEnemies();
}

void EnemyManager::SetCharacterReferences(Vector2* mario, Vector2* luigi, Vector2* active) {
    marioPosition_ = mario;
    luigiPosition_ = luigi;
    activeCharacterPosition_ = active;
    
    // Update all existing enemies with the new character reference for AI
    for (Enemy* enemy : enemies) {
        if (enemy) {
            enemy->SetPlayerReference(activeCharacterPosition_);
        }
    }
}

// Simplified Draw method following ObjectManager pattern
void EnemyManager::Draw() {
    for (Enemy* enemy : enemies) {
        if (enemy && enemy->IsAlive()) {
            enemy->Draw();
        }
    }
}

// Keep SetFrameCount for consistency with GameObject pattern
void EnemyManager::SetFrameCount() {
    Enemy::SetFrameCount(); // Call static method like ObjectManager does
}

void EnemyManager::UpdateEnemyBehaviors(float deltaTime) {
    (void)deltaTime; // Suppress unused parameter warning
    for (Enemy* enemy : enemies) {
        if (enemy && enemy->IsAlive()) {
            UpdateEnemyTargeting(enemy);
        }
    }
}

void EnemyManager::UpdateEnemyTargeting(Enemy* enemy) {
    if (!enemy || !activeCharacterPosition_) return;
    
    float distanceToPlayer = enemy->GetDistanceToPlayer();
    
    // Update enemy behavior based on distance to player
    if (distanceToPlayer <= 100.0f) {
        // Player is close - enemy becomes more aggressive
        if (enemy->CanAttack() && enemy->GetState() == EnemyState::Normal) {
            enemy->EnterAttackMode();
        }
    }
}

void EnemyManager::UpdateEnemyAI(float deltaTime) {
    // Advanced AI updates can be implemented here
    // For now, we update basic behaviors
    
    for (Enemy* enemy : enemies) {
        if (enemy && enemy->IsAlive()) {
            // Update enemy state machine
            switch (enemy->GetState()) {
                case EnemyState::Attacking:
                    if (enemy->CanAttack()) {
                        enemy->PerformAttack();
                    }
                    break;
                case EnemyState::Stunned:
                    // Stunned enemies don't move
                    break;
                default:
                    // Normal behavior handled by movement strategy
                    break;
            }
        }
    }
}

void EnemyManager::UpdateBossAI(float deltaTime) {
    if (!currentBoss_ || !currentBoss_->IsAlive()) return;
    
    Bowser* bowser = dynamic_cast<Bowser*>(currentBoss_);
    if (bowser) {
        // Check if boss should enter rage mode
        float healthPercentage = bowser->GetHP() / bowser->GetMaxHealth();
        if (healthPercentage <= 0.3f) {
            // Boss enters rage mode - could set a different movement strategy
            bowser->EnterRageMode();
        }
    }
}

// Advanced enemy management methods (non-collision related)
void EnemyManager::SetDifficulty(float difficultyMultiplier) {
    difficultyMultiplier_ = difficultyMultiplier;
    
    // Adjust enemy speeds and behaviors
    for (Enemy* enemy : enemies) {
        if (enemy && enemy->IsAlive()) {
            ModifyEnemySpeed(enemy, difficultyMultiplier);
        }
    }
}

void EnemyManager::SetGlobalSpeedMultiplier(float multiplier) {
    globalSpeedMultiplier_ = multiplier;
}

void EnemyManager::PauseAllEnemies() {
    isPaused_ = true;
    
    for (Enemy* enemy : enemies) {
        if (enemy && enemy->IsAlive()) {
            MovementStrategy* currentStrategy = enemy->GetMovementStrategy();
            if (currentStrategy) {
                pausedStrategies_[enemy] = currentStrategy;
                // enemy->SetMovementStrategy(new StationaryMovement(0.0f)); // Commented out - need to implement StationaryMovement
            }
        }
    }
}

void EnemyManager::ResumeAllEnemies() {
    isPaused_ = false;
    
    for (auto& pair : pausedStrategies_) {
        Enemy* enemy = pair.first;
        MovementStrategy* originalStrategy = pair.second;
        
        if (enemy && enemy->IsAlive() && originalStrategy) {
            // enemy->SetMovementStrategy(originalStrategy->Clone()); // Commented out - need Clone method
        }
    }
    pausedStrategies_.clear();
}

void EnemyManager::SpawnBoss(EnemyType bossType, Vector2 position) {
    if (bossType == EnemyType::Bowser) {
        SpawnEnemy(bossType, position, 0);
    }
}

void EnemyManager::SetBossRageMode() {
    if (HasBoss()) {
        Bowser* bowser = dynamic_cast<Bowser*>(currentBoss_);
        if (bowser) {
            bowser->EnterRageMode();
        }
    }
}

void EnemyManager::DamageEnemiesInRadius(Vector2 center, float radius, int damage) {
    std::vector<Enemy*> nearbyEnemies = GetEnemiesInRadius(center, radius);
    
    for (Enemy* enemy : nearbyEnemies) {
        enemy->OnProjectileHit(center, damage);
    }
}

void EnemyManager::KnockbackEnemiesInRadius(Vector2 center, float radius, float force) {
    std::vector<Enemy*> nearbyEnemies = GetEnemiesInRadius(center, radius);
    
    for (Enemy* enemy : nearbyEnemies) {
        Vector2 enemyPos = enemy->GetPosition();
        Vector2 direction = {enemyPos.x - center.x, enemyPos.y - center.y};
        float distance = sqrt(direction.x * direction.x + direction.y * direction.y);
        
        if (distance > 0) {
            direction.x /= distance;
            direction.y /= distance;
            
            Vector2 knockback = {direction.x * force, direction.y * force * 0.5f};
            Vector2 currentVel = enemy->GetVelocity();
            enemy->SetVelocity({currentVel.x + knockback.x, currentVel.y + knockback.y});
        }
    }
}

void EnemyManager::SpawnFormation(EnemyType type, Vector2 startPos, int count, float spacing) {
    for (int i = 0; i < count; i++) {
        Vector2 spawnPos = {startPos.x + i * spacing, startPos.y};
        SpawnEnemy(type, spawnPos, 0);
    }
}

void EnemyManager::SpawnWave(const std::vector<std::pair<EnemyType, Vector2>>& wave) {
    for (const auto& enemy : wave) {
        SpawnEnemy(enemy.first, enemy.second, 0);
    }
}

void EnemyManager::SpawnPatrolGroup(EnemyType type, Vector2 pointA, Vector2 pointB, int count) {
    for (int i = 0; i < count; i++) {
        Vector2 spawnPos = {pointA.x + (pointB.x - pointA.x) * i / (count - 1), pointA.y};
        Enemy* enemy = CreateEnemyByType(type, spawnPos, 0);
        if (enemy) {
            // enemy->SetMovementStrategy(new PatrolMovement(pointA, pointB)); // Commented out - need PatrolMovement
            AddEnemy(enemy);
        }
    }
}

void EnemyManager::MakeEnemyFollow(Enemy* enemy, Vector2* target, float speed) {
    if (enemy) {
        // enemy->SetMovementStrategy(new FollowingMovement(target, speed)); // Commented out - need FollowingMovement
    }
}

void EnemyManager::MakeEnemyPatrol(Enemy* enemy, Vector2 pointA, Vector2 pointB, float speed) {
    if (enemy) {
        // enemy->SetMovementStrategy(new PatrolMovement(pointA, pointB, speed)); // Commented out - need PatrolMovement
    }
}

void EnemyManager::MakeEnemyJump(Enemy* enemy, float interval, float force) {
    if (enemy) {
        // enemy->SetMovementStrategy(new JumpingMovement(interval, force)); // Commented out - need JumpingMovement
    }
}

// Utility functions
size_t EnemyManager::GetEnemyCount() const {
    return enemies.size();
}

size_t EnemyManager::GetAliveEnemyCount() const {
    return std::count_if(enemies.begin(), enemies.end(), 
        [](const Enemy* enemy) { return enemy && enemy->IsAlive(); });
}

std::vector<Enemy*> EnemyManager::GetEnemiesInRadius(Vector2 center, float radius) const {
    std::vector<Enemy*> nearbyEnemies;
    
    for (Enemy* enemy : enemies) {
        if (enemy && enemy->IsAlive()) {
            Vector2 enemyPos = enemy->GetPosition();
            float distance = sqrt(pow(enemyPos.x - center.x, 2) + pow(enemyPos.y - center.y, 2));
            
            if (distance <= radius) {
                nearbyEnemies.push_back(enemy);
            }
        }
    }
    
    return nearbyEnemies;
}

std::vector<Enemy*> EnemyManager::GetEnemiesByType(EnemyType type) const {
    std::vector<Enemy*> typeEnemies;
    
    for (Enemy* enemy : enemies) {
        if (enemy && enemy->IsAlive() && enemy->GetType() == type) {
            typeEnemies.push_back(enemy);
        }
    }
    
    return typeEnemies;
}

bool EnemyManager::AreAllEnemiesDead() const {
    return GetAliveEnemyCount() == 0;
}

void EnemyManager::ModifyEnemySpeed(Enemy* enemy, float speedMultiplier) {
    if (!enemy) return;
    
    // This would need to be implemented based on movement strategy
    // For now, we can modify velocity directly
    Vector2 currentVel = enemy->GetVelocity();
    enemy->SetVelocity({currentVel.x * speedMultiplier, currentVel.y});
}

// Remaining utility methods with placeholders
void EnemyManager::StunAllEnemiesInRadius(Vector2 center, float radius, float duration) {
    std::vector<Enemy*> nearbyEnemies = GetEnemiesInRadius(center, radius);
    for (Enemy* enemy : nearbyEnemies) {
        enemy->Stun(duration);
    }
}

void EnemyManager::HandleProjectileCollisions(Vector2 projectilePos, float projectileRadius, int damage) {
    DamageEnemiesInRadius(projectilePos, projectileRadius, damage);
}

void EnemyManager::EnableAggressiveMode(bool aggressive) {
    for (Enemy* enemy : enemies) {
        if (enemy && enemy->IsAlive()) {
            ModifyEnemyAggression(enemy, aggressive);
        }
    }
}

void EnemyManager::SetEnemyTarget(Enemy* enemy, Vector2* target) {
    if (enemy) {
        enemy->SetPlayerReference(target);
    }
}

std::vector<Enemy*> EnemyManager::GetEnemiesInState(EnemyState state) const {
    std::vector<Enemy*> stateEnemies;
    for (Enemy* enemy : enemies) {
        if (enemy && enemy->IsAlive() && enemy->GetState() == state) {
            stateEnemies.push_back(enemy);
        }
    }
    return stateEnemies;
}

bool EnemyManager::HasEnemyType(EnemyType type) const {
    return !GetEnemiesByType(type).empty();
}

Enemy* EnemyManager::GetNearestEnemyToPosition(Vector2 position) const {
    Enemy* nearest = nullptr;
    float minDistance = std::numeric_limits<float>::max();
    
    for (Enemy* enemy : enemies) {
        if (enemy && enemy->IsAlive()) {
            Vector2 enemyPos = enemy->GetPosition();
            float distance = sqrt(pow(enemyPos.x - position.x, 2) + pow(enemyPos.y - position.y, 2));
            if (distance < minDistance) {
                minDistance = distance;
                nearest = enemy;
            }
        }
    }
    
    return nearest;
}

Enemy* EnemyManager::GetFarthestEnemyFromPosition(Vector2 position) const {
    Enemy* farthest = nullptr;
    float maxDistance = 0.0f;
    
    for (Enemy* enemy : enemies) {
        if (enemy && enemy->IsAlive()) {
            Vector2 enemyPos = enemy->GetPosition();
            float distance = sqrt(pow(enemyPos.x - position.x, 2) + pow(enemyPos.y - position.y, 2));
            if (distance > maxDistance) {
                maxDistance = distance;
                farthest = enemy;
            }
        }
    }
    
    return farthest;
}

void EnemyManager::ApplyStunEffect(Vector2 center, float radius, float duration) {
    StunAllEnemiesInRadius(center, radius, duration);
}

void EnemyManager::ApplyKnockbackEffect(Vector2 center, float radius, float force) {
    KnockbackEnemiesInRadius(center, radius, force);
}

void EnemyManager::ApplySlowEffect(float duration, float speedMultiplier) {
    (void)duration; (void)speedMultiplier; // Placeholder implementation
}

void EnemyManager::ApplyFreezeEffect(float duration) {
    (void)duration; // Placeholder implementation
}

void EnemyManager::ModifyEnemyAggression(Enemy* enemy, bool aggressive) {
    (void)enemy; (void)aggressive; // Placeholder implementation
}

void EnemyManager::ModifyEnemyDetectionRange(Enemy* enemy, float newRange) {
    (void)enemy; (void)newRange; // Placeholder implementation
}

void EnemyManager::DebugDrawEnemyInfo() const {
    // Debug visualization placeholder
}

void EnemyManager::PrintEnemyStates() const {
    std::cout << "Enemy States:" << std::endl;
    for (size_t i = 0; i < enemies.size(); ++i) {
        Enemy* enemy = enemies[i];
        if (enemy) {
            std::cout << "Enemy " << i << ": Type=" << static_cast<int>(enemy->GetType()) 
                      << ", State=" << static_cast<int>(enemy->GetState()) 
                      << ", Alive=" << enemy->IsAlive() << std::endl;
        }
    }
}

void EnemyManager::UpdateEnemyFormations(float deltaTime) {
    (void)deltaTime; // Placeholder implementation
}
