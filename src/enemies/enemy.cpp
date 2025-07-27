#include "include/enemies/enemy.hpp"
#include "include/enemies/movement_strategy.hpp"
#include <cmath>
#include <algorithm>

Enemy::Enemy(Vector2 pos, int w, int h, EnemyType type, int spriteId)
    : position(pos), width(w), height(h), type(type), spriteId(spriteId),
      alive(true), velocity({0, 0}), movementStrategy_(nullptr), 
      health(1.0f), maxHealth(1.0f), stunTimer(0.0f), invulnerabilityTimer(0.0f),
      facingLeft(true), detectionRange(80.0f), playerPosition(nullptr),
      isAggressive(false), aggroRange(60.0f), attackCooldown(1.0f), attackTimer(0.0f),
      state(EnemyState::Normal) {}

Enemy::~Enemy() {
    delete movementStrategy_;
}

void Enemy::Update(float dt) {
    if (!alive) return;
    
    // Update timers
    UpdateTimers(dt);
    
    // Update player detection
    UpdatePlayerDetection();
    
    // Use strategy pattern for movement
    UpdateMovement(dt);
    
    // Apply physics
    ApplyGravity(dt);
    HandleGroundCollision();
}

void Enemy::UpdateTimers(float deltaTime) {
    if (stunTimer > 0) {
        stunTimer -= deltaTime;
        if (stunTimer <= 0) {
            if (state == EnemyState::Stunned) {
                state = EnemyState::Normal;
            }
        }
    }
    
    if (invulnerabilityTimer > 0) {
        invulnerabilityTimer -= deltaTime;
    }
    
    if (attackTimer > 0) {
        attackTimer -= deltaTime;
    }
}

void Enemy::UpdatePlayerDetection() {
    if (!playerPosition) return;
    
    float distanceToPlayer = GetDistanceToPlayer();
    
    if (distanceToPlayer <= detectionRange && !isAggressive) {
        isAggressive = true;
        OnPlayerDetected();
    } else if (distanceToPlayer > aggroRange && isAggressive) {
        isAggressive = false;
        OnPlayerLost();
    }
    
    // Update facing direction based on player position
    if (isAggressive && playerPosition) {
        facingLeft = (playerPosition->x < position.x);
    }
}

void Enemy::SetMovementStrategy(MovementStrategy* strategy) {
    delete movementStrategy_;
    movementStrategy_ = strategy;
}

MovementStrategy* Enemy::GetMovementStrategy() const {
    return movementStrategy_;
}

void Enemy::UpdateMovement(float dt) {
    if (movementStrategy_ && state != EnemyState::Stunned) {
        movementStrategy_->Update(this, dt);
    }
}

void Enemy::ApplyGravity(float deltaTime) {
    const float gravity = 400.0f;
    velocity.y += gravity * deltaTime;
}

void Enemy::HandleGroundCollision(float groundLevel) {
    if (position.y + height >= groundLevel) {
        position.y = groundLevel - height;
        if (velocity.y > 0) {
            velocity.y = 0;
        }
    }
}

void Enemy::HandleWallCollision() {
    // Simple wall collision - reverse direction
    ReverseDirection();
}

void Enemy::ReverseDirection() {
    velocity.x *= -1;
    facingLeft = !facingLeft;
    
    // Also reverse strategy direction if it supports it
    if (movementStrategy_) {
        movementStrategy_->ReverseDirection();
    }
}

void Enemy::OnCharacterInteraction(Vector2 characterPos, bool characterFalling) {
    if (!alive || IsInvulnerable()) return;
    
    Vector2 characterCenter = {characterPos.x, characterPos.y};
    Vector2 enemyCenter = {position.x + width/2, position.y + height/2};
    
    // Determine collision type
    float deltaY = characterCenter.y - enemyCenter.y;
    
    if (characterFalling && deltaY < -height/4) {
        // Character hit from above
        OnHitFromAbove();
    } else {
        // Character hit from side or below
        OnHitFromSide();
    }
}

void Enemy::OnProjectileHit(Vector2 projectilePos, int damage) {
    if (!alive || IsInvulnerable()) return;
    
    DealDamage((float)damage);
    invulnerabilityTimer = 0.5f; // Brief invulnerability
    
    // Knockback effect
    Vector2 direction = {position.x - projectilePos.x, 0};
    float distance = sqrt(direction.x * direction.x + direction.y * direction.y);
    if (distance > 0) {
        direction.x /= distance;
        velocity.x += direction.x * 50.0f; // Knockback force
    }
}

void Enemy::OnEnvironmentCollision(Vector2 collisionPoint) {
    // Handle collision with environment objects
    // This could trigger different behaviors based on what was hit
    (void)collisionPoint; // Suppress unused parameter warning for now
    
    // Default behavior: reverse direction
    if (ShouldReverseDirection()) {
        ReverseDirection();
    }
}

void Enemy::Stun(float duration) {
    state = EnemyState::Stunned;
    stunTimer = duration;
    velocity.x = 0; // Stop horizontal movement
}

void Enemy::EnterAttackMode() {
    if (state == EnemyState::Normal) {
        state = EnemyState::Attacking;
        attackTimer = attackCooldown;
    }
}

void Enemy::ExitAttackMode() {
    if (state == EnemyState::Attacking) {
        state = EnemyState::Normal;
    }
}

bool Enemy::IsPlayerInRange(float range) const {
    if (!playerPosition) return false;
    return GetDistanceToPlayer() <= range;
}

Vector2 Enemy::GetDirectionToPlayer() const {
    if (!playerPosition) return {0, 0};
    
    Vector2 direction = {playerPosition->x - position.x, playerPosition->y - position.y};
    float distance = sqrt(direction.x * direction.x + direction.y * direction.y);
    
    if (distance > 0) {
        direction.x /= distance;
        direction.y /= distance;
    }
    
    return direction;
}

float Enemy::GetDistanceToPlayer() const {
    if (!playerPosition) return INFINITY;
    
    float dx = playerPosition->x - position.x;
    float dy = playerPosition->y - position.y;
    return sqrt(dx * dx + dy * dy);
}

void Enemy::DealDamage(float damage) {
    health -= damage;
    if (health <= 0) {
        health = 0;
        alive = false;
        state = EnemyState::Dead;
    }
}

bool Enemy::ShouldReverseDirection() const {
    // Check if enemy should reverse direction due to obstacles
    // This is a simplified check - in a full implementation, you'd check for walls/pits
    return (position.x <= 50.0f || position.x >= 750.0f);
}

Rectangle Enemy::GetRect() const {
    return {position.x, position.y, (float)width, (float)height};
}

Vector2 Enemy::GetPosition() const {
    return position;
}

Vector2 Enemy::GetVelocity() const {
    return velocity;
}

EnemyType Enemy::GetType() const {
    return type;
}

EnemyState Enemy::GetState() const {
    return state;
}

bool Enemy::IsAlive() const {
    return alive;
}

void Enemy::SetPosition(Vector2 pos) {
    position = pos;
}

void Enemy::SetVelocity(Vector2 vel) {
    velocity = vel;
}

void Enemy::SetAlive(bool status) {
    alive = status;
    if (!alive) {
        state = EnemyState::Dead;
    }
}

void Enemy::SetState(EnemyState newState) {
    state = newState;
}

bool Enemy::CheckCollision(Rectangle other) const {
    return CheckCollisionRecs(GetRect(), other);
}

void Enemy::Render(Texture &tex, const std::unordered_map<int, Rectangle> &spriteRects) const {
    if (!alive) return;
    
    auto it = spriteRects.find(spriteId);
    if (it != spriteRects.end()) {
        Rectangle sourceRect = it->second;
        
        // Flip sprite based on facing direction
        if (facingLeft) {
            sourceRect.width = -abs(sourceRect.width);
        } else {
            sourceRect.width = abs(sourceRect.width);
        }
        
        // Apply visual effects based on state
        Color tint = WHITE;
        if (state == EnemyState::Stunned) {
            tint = GRAY;
        } else if (IsInvulnerable()) {
            tint = ColorAlpha(WHITE, 0.5f);
        }
        
        DrawTextureRec(tex, sourceRect, position, tint);
    }
}
