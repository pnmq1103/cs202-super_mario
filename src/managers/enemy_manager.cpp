#include "include/managers/enemy_manager.hpp"
#include "include/enemies/goomba.hpp"
#include "include/enemies/koopa_troopa.hpp"
#include "include/enemies/piranha_plant.hpp"
#include "include/enemies/bowser.hpp"
#include "include/enemies/movement_strategy.hpp"
#include "include/characters/character.hpp"
#include <algorithm>
#include <cmath>
#include <iostream>

EnemyManager::EnemyManager() 
    : marioPosition_(nullptr), luigiPosition_(nullptr), activeCharacterPosition_(nullptr),
      difficultyMultiplier_(1.0f), isPaused_(false), globalSpeedMultiplier_(1.0f),
      currentBoss_(nullptr), interactionCooldown_(0.1f) {}

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
    
    switch (type) {
        case EnemyType::Goomba:
            enemy = new Goomba(position, spriteId);
            break;
        case EnemyType::Koopa:
            enemy = new KoopaTroopa(position, spriteId);
            break;
        case EnemyType::Piranha:
            enemy = new PiranhaPlant(position, spriteId);
            break;
        case EnemyType::Bowser:
            enemy = new Bowser(position, spriteId);
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

void EnemyManager::ClearAllEnemies() {
    for (Enemy* enemy : enemies) {
        delete enemy;
    }
    enemies.clear();
    currentBoss_ = nullptr;
    pausedStrategies_.clear();
    enemyInteractionTimers_.clear();
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
    
    // Apply global speed multiplier
    float adjustedDeltaTime = deltaTime * globalSpeedMultiplier_;
    
    UpdateInteractionTimers(adjustedDeltaTime);
    
    // Update each enemy
    for (Enemy* enemy : enemies) {
        if (enemy && enemy->IsAlive()) {
            enemy->Update(adjustedDeltaTime);
        }
    }
    
    // Update AI behaviors
    UpdateEnemyAI(adjustedDeltaTime);
    UpdateEnemyBehaviors(adjustedDeltaTime);
    
    // Update boss AI
    if (HasBoss()) {
        UpdateBossAI(adjustedDeltaTime);
    }
    
    // Handle environmental interactions
    HandleEnvironmentalHazards(adjustedDeltaTime);
}

void EnemyManager::UpdateInteractionTimers(float deltaTime) {
    for (auto& pair : enemyInteractionTimers_) {
        pair.second -= deltaTime;
    }
}

void EnemyManager::SetCharacterReferences(Vector2* mario, Vector2* luigi, Vector2* active) {
    marioPosition_ = mario;
    luigiPosition_ = luigi;
    activeCharacterPosition_ = active;
    
    // Update all existing enemies with the new character reference
    for (Enemy* enemy : enemies) {
        if (enemy) {
            enemy->SetPlayerReference(activeCharacterPosition_);
        }
    }
}

void EnemyManager::HandleCharacterInteractions(Character* activeCharacter) {
    if (!activeCharacter) return;
    
    CheckCollisionWithCharacter(activeCharacter);
}

void EnemyManager::CheckCollisionWithCharacter(Character* character) {
    if (!character) return;
    
    Rectangle characterRect = character->GetRectangle();
    
    for (Enemy* enemy : enemies) {
        if (enemy && enemy->IsAlive() && CanInteractWithCharacter(enemy)) {
            if (enemy->CheckCollision(characterRect)) {
                ProcessEnemyCharacterCollision(enemy, character);
                enemyInteractionTimers_[enemy] = interactionCooldown_;
            }
        }
    }
}

bool EnemyManager::CanInteractWithCharacter(Enemy* enemy) const {
    auto it = enemyInteractionTimers_.find(enemy);
    return (it == enemyInteractionTimers_.end() || it->second <= 0.0f);
}

void EnemyManager::ProcessEnemyCharacterCollision(Enemy* enemy, Character* character) {
    Rectangle characterRect = character->GetRectangle();
    Rectangle enemyRect = enemy->GetRect();
    
    Vector2 characterCenter = {characterRect.x + characterRect.width/2, 
                              characterRect.y + characterRect.height/2};
    Vector2 enemyCenter = {enemyRect.x + enemyRect.width/2, 
                          enemyRect.y + enemyRect.height/2};
    
    // Determine collision type
    float deltaY = characterCenter.y - enemyCenter.y;
    bool characterFromAbove = (deltaY < -enemyRect.height/4) && character->IsFalling();
    
    if (characterFromAbove) {
        // Character hit enemy from above
        enemy->OnHitFromAbove();
        character->Bounce(); // Make character bounce
        
        // Award points based on enemy type
        int points = 100;
        switch (enemy->GetType()) {
            case EnemyType::Goomba: points = 100; break;
            case EnemyType::Koopa: points = 200; break;
            case EnemyType::Piranha: points = 300; break;
            case EnemyType::Bowser: points = 1000; break;
            default: points = 100; break;
        }
        // Note: Points would be added through GameManaging
        
    } else {
        // Character hit enemy from side
        if (character->IsStarman()) {
            // Star power destroys enemies
            enemy->OnHitFromSide();
        } else {
            // Enemy damages character
            enemy->OnHitFromSide();
            character->Die();
        }
    }
}

void EnemyManager::RenderAll(Texture& enemyTexture, const std::unordered_map<int, Rectangle>& spriteRects) {
    for (Enemy* enemy : enemies) {
        if (enemy && enemy->IsAlive()) {
            enemy->Render(enemyTexture, spriteRects);
        }
    }
}

void EnemyManager::HandleWallCollisions() {
    CheckWallCollisions();
}

void EnemyManager::CheckWallCollisions(float leftWall, float rightWall) {
    for (Enemy* enemy : enemies) {
        if (enemy && enemy->IsAlive()) {
            Vector2 pos = enemy->GetPosition();
            Rectangle rect = enemy->GetRect();
            
            if (pos.x <= leftWall || pos.x + rect.width >= rightWall) {
                enemy->HandleWallCollision();
            }
        }
    }
}

void EnemyManager::CheckGroundCollisions(float groundLevel) {
    for (Enemy* enemy : enemies) {
        if (enemy && enemy->IsAlive()) {
            enemy->HandleGroundCollision(groundLevel);
        }
    }
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
    WalkingMovement* walkingStrategy = dynamic_cast<WalkingMovement*>(enemy->GetMovementStrategy());
    if (walkingStrategy && distanceToPlayer <= 100.0f) {
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
        float healthPercentage = bowser->getHP() / bowser->GetMaxHealth();
        if (healthPercentage <= 0.3f) {
            // Boss enters rage mode - could set a different movement strategy
            bowser->EnterRageMode();
        }
    }
}

void EnemyManager::HandleEnvironmentalHazards(float deltaTime) {
    (void)deltaTime; // Suppress unused parameter warning
    CheckGroundCollisions();
    CheckWallCollisions();
}

// Advanced enemy management methods
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
                enemy->SetMovementStrategy(new StationaryMovement(0.0f));
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
            enemy->SetMovementStrategy(originalStrategy->Clone());
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
            enemy->SetMovementStrategy(new PatrolMovement(pointA, pointB));
            AddEnemy(enemy);
        }
    }
}

void EnemyManager::MakeEnemyFollow(Enemy* enemy, Vector2* target, float speed) {
    if (enemy) {
        enemy->SetMovementStrategy(new FollowingMovement(target, speed));
    }
}

void EnemyManager::MakeEnemyPatrol(Enemy* enemy, Vector2 pointA, Vector2 pointB, float speed) {
    if (enemy) {
        enemy->SetMovementStrategy(new PatrolMovement(pointA, pointB, speed));
    }
}

void EnemyManager::MakeEnemyJump(Enemy* enemy, float interval, float force) {
    if (enemy) {
        enemy->SetMovementStrategy(new JumpingMovement(interval, force));
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
    
    WalkingMovement* walkingStrategy = dynamic_cast<WalkingMovement*>(enemy->GetMovementStrategy());
    if (walkingStrategy) {
        walkingStrategy->SetSpeed(walkingStrategy->GetSpeed() * speedMultiplier);
    }
}
