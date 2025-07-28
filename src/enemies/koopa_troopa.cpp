#include "include/enemies/koopa_troopa.hpp"
#include "include/enemies/movement_strategy.hpp"

KoopaTroopa::KoopaTroopa(Vector2 pos, int spriteId)
    : Enemy(pos, 16, 24, EnemyType::Koopa, spriteId), state(KoopaState::Walking), shellTimer(0.0f) {
    // Set up basic walking movement for Koopa
    SetMovementStrategy(new WalkingMovement(40.0f)); // 40 pixels per second
    
    // Koopa-specific properties
    health = 1.0f;
    maxHealth = 1.0f;
    detectionRange = 100.0f;
}

void KoopaTroopa::Update(float dt) {
    if (!alive) return;
    
    // Update shell timer
    if (state == KoopaState::Shell) {
        shellTimer += dt;
        // After 5 seconds in shell, come back out
        if (shellTimer >= 5.0f) {
            state = KoopaState::Walking;
            shellTimer = 0.0f;
            SetState(EnemyState::Normal); // Set the base Enemy state
            SetMovementStrategy(new WalkingMovement(40.0f));
        }
    }
    
    // Call base enemy update
    Enemy::Update(dt);
}

void KoopaTroopa::OnHitFromAbove() {
    if (state == KoopaState::Walking) {
        // Enter shell mode
        state = KoopaState::Shell;
        SetState(EnemyState::Shell); // Set the base Enemy state
        velocity = {0.0f, 0.0f};
        shellTimer = 0.0f;
        
        // Remove movement strategy while in shell
        if (movementStrategy_) {
            delete movementStrategy_;
            movementStrategy_ = nullptr;
        }
    } else if (state == KoopaState::Shell) {
        // Kick the shell
        state = KoopaState::Sliding;
        velocity.x = facingLeft ? -200.0f : 200.0f; // Slide fast
    }
}

void KoopaTroopa::OnHitFromSide() {
    if (state == KoopaState::Shell || state == KoopaState::Sliding) {
        // Shell can be destroyed by fireballs
        alive = false;
        SetState(EnemyState::Dead); // Set the base Enemy state
    } else {
        // Walking Koopa enters shell
        OnHitFromAbove();
    }
}

Enemy* KoopaTroopa::Clone() const {
    KoopaTroopa* clone = new KoopaTroopa(*this);
    // Clone the movement strategy
    if (movementStrategy_) {
        clone->SetMovementStrategy(movementStrategy_->Clone());
    }
    return clone;
}

KoopaState KoopaTroopa::GetState() const {
    return state;
}
