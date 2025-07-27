#include "include/enemies/koopa_troopa.hpp"
#include "include/enemies/movement_strategy.hpp"

KoopaTroopa::KoopaTroopa(Vector2 pos, int spriteId)
    : Enemy(pos, 16, 16, EnemyType::Koopa, spriteId),
      state(KoopaState::Walking) {
  // Set up walking movement strategy initially
  SetMovementStrategy(new WalkingMovement(20.0f, true)); // Speed: 20, moving left
}

void KoopaTroopa::Update(float dt) {
  if (!alive)
    return;

  switch (state) {
    case KoopaState::Walking:
      // Use walking strategy
      Enemy::Update(dt);
      break;

    case KoopaState::Shell:
      shellTimer += dt;
      if (shellTimer > 5.0f) {
        state = KoopaState::Walking;
        shellTimer = 0;
        // Switch back to walking strategy
        SetMovementStrategy(new WalkingMovement(20.0f, true));
      }
      // No movement when in shell state
      break;

    case KoopaState::Sliding:
      // Use sliding strategy for fast movement
      Enemy::Update(dt);
      break;
  }
}

void KoopaTroopa::OnHitFromAbove() {
  if (state == KoopaState::Walking) {
    state = KoopaState::Shell;
    shellTimer = 0;
    // Remove movement strategy (stationary shell)
    SetMovementStrategy(nullptr);
  } else if (state == KoopaState::Shell) {
    state = KoopaState::Sliding;
    // Switch to sliding strategy
    bool movingLeft = (rand() % 2 == 0); // Random direction
    SetMovementStrategy(new SlidingMovement(80.0f, movingLeft));
  } else {
    state = KoopaState::Shell;
    SetMovementStrategy(nullptr);
  }
}

void KoopaTroopa::OnHitFromSide() {
  if (state == KoopaState::Sliding) {
    // Reverse sliding direction
    ReverseDirection();
  } else {
    alive = false;
  }
}

Enemy *KoopaTroopa::Clone() const {
  KoopaTroopa *clone = new KoopaTroopa(*this);
  // Clone the movement strategy
  if (movementStrategy_) {
    clone->SetMovementStrategy(movementStrategy_->Clone());
  }
  return clone;
}

KoopaState KoopaTroopa::GetState() const {
  return state;
}
