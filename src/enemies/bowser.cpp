#include "include/enemies/bowser.hpp"
#include "include/enemies/movement_strategy.hpp"

Bowser::Bowser(Vector2 pos, int spriteId)
    : Enemy(pos, 32, 48, EnemyType::Bowser, spriteId), hp(5), fireTimer(0.0f) {
    // Set up boss movement pattern
    SetMovementStrategy(new WalkingMovement(30.0f)); // Slower than other enemies
    
    // Boss-specific properties
    health = 5.0f;
    maxHealth = 5.0f;
    hp = 5;
    detectionRange = 150.0f;
    aggroRange = 200.0f;
    attackCooldown = 2.0f;
}

void Bowser::Update(float dt) {
    if (!alive) return;
    
    // Update fire timer
    fireTimer += dt;
    
    // Boss AI - more complex behavior
    if (playerPosition && IsPlayerInRange(detectionRange)) {
        // Face the player
        bool shouldFaceLeft = (playerPosition->x < position.x);
        SetFacing(shouldFaceLeft);
        
        // Attack periodically
        if (fireTimer >= attackCooldown && attackTimer <= 0.0f) {
            PerformSpecialAttack();
            attackTimer = attackCooldown;
            fireTimer = 0.0f;
        }
    }
    
    // Call base enemy update
    Enemy::Update(dt);
}

void Bowser::OnHitFromAbove() {
    // Boss is harder to defeat - takes multiple hits
    DealDamage(1.0f);
    hp--;
    
    if (hp <= 0) {
        alive = false;
        state = EnemyState::Dead;
    } else {
        // Enter rage mode when low on health
        if (hp <= 2) {
            EnterRageMode();
        }
        
        // Brief stun when hit
        Stun(1.0f);
    }
}

void Bowser::OnHitFromSide() {
    // Bowser is resistant to side attacks, but still takes some damage
    DealDamage(0.5f);
    hp--;
    
    if (hp <= 0) {
        alive = false;
        state = EnemyState::Dead;
    }
}

Enemy* Bowser::Clone() const {
    Bowser* clone = new Bowser(*this);
    // Clone the movement strategy
    if (movementStrategy_) {
        clone->SetMovementStrategy(movementStrategy_->Clone());
    }
    return clone;
}

int Bowser::getHP() const {
    return hp;
}

bool Bowser::IsVulnerable() const {
    return invulnerabilityTimer <= 0.0f && state != EnemyState::Stunned;
}

void Bowser::EnterRageMode() {
    // Increase speed and reduce attack cooldown when in rage mode
    if (movementStrategy_) {
        delete movementStrategy_;
        SetMovementStrategy(new WalkingMovement(60.0f)); // Faster movement
    }
    attackCooldown = 1.0f; // Attack more frequently
    
    // Visual indicator
    state = EnemyState::Attacking;
}

void Bowser::PerformSpecialAttack() {
    // Boss special attack - could be fire breathing, ground pound, etc.
    // For now, just enter attack mode briefly
    EnterAttackMode();
    
    // Could spawn projectiles, create area effects, etc.
    // This is a placeholder for more complex boss attacks
}

Vector2 Bowser::GetCenterPosition() const {
    return {
        position.x + width / 2.0f,
        position.y + height / 2.0f
    };
}
