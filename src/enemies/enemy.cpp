#include "include/enemies/enemy.hpp"
#include "include/enemies/movement_strategy.hpp"
#include <raylib.h>
#include <cmath>

Enemy::Enemy(
    Vector2 pos, int w, int h, EnemyType enemyType, int spriteId, float health,
    float maxHealth, float stunTimer, float invulnerabilityTimer, bool facingLeft,
    float detectionRange)
    : position(pos),
      velocity({0.0f, 0.0f}),
      width(w),
      height(h),
      alive(true),
      spriteId(spriteId),
      type(enemyType),
      state(EnemyState::Normal),
      movementStrategy_(nullptr),
      health(health),
      maxHealth(maxHealth),
      stunTimer(stunTimer),
      invulnerabilityTimer(invulnerabilityTimer),
      facingLeft(facingLeft),
      detectionRange(detectionRange),
      playerPosition(nullptr),
      isAggressive(false),
      aggroRange(200.0f),
      attackCooldown(1.0f),
      attackTimer(0.0f)
{
}
    
 Enemy::~Enemy() {
    if (movementStrategy_) {
        delete movementStrategy_;
        movementStrategy_ = nullptr;
    }
}

void Enemy::Update(float dt) {
    if (!alive) return;
    
    // Update timers
    UpdateTimers(dt);
    
    // Update player detection
    UpdatePlayerDetection();
    
    // Update movement strategy
    UpdateMovement(dt);
    
    // Apply physics
    ApplyGravity(dt);
    HandleGroundCollision();
}

void Enemy::UpdateTimers(float deltaTime) {
    if (stunTimer > 0.0f) {
        stunTimer -= deltaTime;
        if (stunTimer <= 0.0f) {
            state = EnemyState::Normal;
        }
    }
    
    if (invulnerabilityTimer > 0.0f) {
        invulnerabilityTimer -= deltaTime;
    }
    
    if (attackTimer > 0.0f) {
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
}

void Enemy::SetMovementStrategy(MovementStrategy* strategy) {
    if (movementStrategy_) {
        delete movementStrategy_;
    }
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
    const float gravity = 980.0f; // pixels per second squared
    velocity.y += gravity * deltaTime;
}

void Enemy::HandleGroundCollision(float groundLevel) {
    if (position.y + height >= groundLevel) {
        position.y = groundLevel - height;
        velocity.y = 0.0f;
    }
}

void Enemy::HandleWallCollision() {
    // Simple wall collision - reverse direction
    ReverseDirection();
}

void Enemy::ReverseDirection() {
    velocity.x = -velocity.x;
    facingLeft = !facingLeft;
}

bool Enemy::IsPlayerInRange(float range) const {
    if (!playerPosition) return false;
    return GetDistanceToPlayer() <= range;
}

Vector2 Enemy::GetDirectionToPlayer() const {
    if (!playerPosition) return {0.0f, 0.0f};
    
    Vector2 direction = {
        playerPosition->x - position.x,
        playerPosition->y - position.y
    };
    
    // Normalize
    float length = sqrtf(direction.x * direction.x + direction.y * direction.y);
    if (length > 0.0f) {
        direction.x /= length;
        direction.y /= length;
    }
    
    return direction;
}

float Enemy::GetDistanceToPlayer() const {
    if (!playerPosition) return 9999.0f;
    
    float dx = playerPosition->x - position.x;
    float dy = playerPosition->y - position.y;
    return sqrtf(dx * dx + dy * dy);
}

void Enemy::OnCharacterInteraction(Vector2 characterPos, bool characterFalling) {
    if (invulnerabilityTimer > 0.0f) return;
    
    // Basic interaction - if character is falling and above enemy, stomp
    if (characterFalling && characterPos.y < position.y) {
        OnHitFromAbove();
    } else {
        OnHitFromSide();
    }
}

void Enemy::OnProjectileHit(Vector2 projectilePos, int damage) {
    (void)projectilePos; // Suppress unused parameter warning
    DealDamage(static_cast<float>(damage));
}

void Enemy::OnEnvironmentCollision(Vector2 collisionPoint) {
    (void)collisionPoint; // Suppress unused parameter warning
    // Default implementation - could be overridden for specific behaviors
}

void Enemy::Stun(float duration) {
    state = EnemyState::Stunned;
    stunTimer = duration;
    velocity.x = 0.0f; // Stop horizontal movement
}

void Enemy::EnterAttackMode() {
    if (state != EnemyState::Stunned) {
        state = EnemyState::Attacking;
    }
}

void Enemy::ExitAttackMode() {
    if (state == EnemyState::Attacking) {
        state = EnemyState::Normal;
    }
}

void Enemy::DealDamage(float damage) {
    if (invulnerabilityTimer > 0.0f) return;
    
    health -= damage;
    if (health <= 0.0f) {
        health = 0.0f;
        alive = false;
        state = EnemyState::Dead;
    } else {
        // Become invulnerable briefly after taking damage
        invulnerabilityTimer = 0.5f;
    }
}

bool Enemy::ShouldReverseDirection() const {
    // Basic logic - could be enhanced with more sophisticated checks
    return false;
}

// Getters and setters
Rectangle Enemy::GetRect() const {
    return {position.x, position.y, static_cast<float>(width), static_cast<float>(height)};
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
    Rectangle myRect = GetRect();
    return CheckCollisionRecs(myRect, other);
}

void Enemy::Render(Texture &tex, const std::unordered_map<int, Rectangle> &spriteRects) const {
    (void)tex; // Suppress unused parameter warning
    (void)spriteRects; // Suppress unused parameter warning
    
    // Default implementation - just draw a colored rectangle
    Rectangle rect = GetRect();
    Color color = RED;
    
    switch (type) {
        case EnemyType::Goomba:
            color = BROWN;
            break;
        case EnemyType::Koopa:
            color = GREEN;
            break;
        case EnemyType::Piranha:
            color = DARKGREEN;
            break;
        case EnemyType::Bowser:
            color = ORANGE;
            break;
        default:
            color = RED;
            break;
    }
    
    // Apply visual effects based on state
    if (state == EnemyState::Stunned) {
        color = ColorAlpha(color, 0.7f);
    } else if (IsInvulnerable()) {
        color = ColorAlpha(color, 0.5f);
    }
    
    DrawRectangleRec(rect, color);
}

