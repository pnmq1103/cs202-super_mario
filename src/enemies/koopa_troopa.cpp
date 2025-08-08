#include "include/enemies/koopa_troopa.hpp"
#include "include/enemies/movement_strategy.hpp"

KoopaTroopa::KoopaTroopa(Vector2 pos, float Nscale)
    : Enemy(pos, Nscale, EnemyType::Koopa), koopa_state(KoopaState::Walking),
      shell_timer(0.0f) {
  // Set up basic walking movement for Koopa
  SetMovementStrategy(new WalkingMovement(5.0f)); // 40 pixels per second

  // Koopa-specific properties
  health          = 1.0f;
  max_health      = 1.0f;
  detection_range = 100.0f;

  // Set initial velocity for walking
  velocity.x  = -10.0f; // Start walking left
  facing_left = true;

  position.y -= 27 * scale;

  std::cout << "Koopa spawned with initial velocity: " << velocity.x
            << std::endl;
}

KoopaTroopa::~KoopaTroopa() {}

void KoopaTroopa::OnHit() {
  return;
}

void KoopaTroopa::Update() {
  if (!alive)
    return;
  if (shell_timer > 0.0f) {
    shell_timer -= GetFrameTime();
  }
  // Call base enemy update
  Enemy::Update();
}

void KoopaTroopa::OnHitFromAbove() {
  if (shell_timer > 0.0f) {
    return; // Ignore hits while in shell mode
  }
  switch (GetKoopaState()) {
    case KoopaState::Walking: {
      koopa_state = KoopaState::Sliding;
      state       = EnemyState::Shell; // Set the base Enemy state

      SetMovementStrategy(new SlidingMovement());

      shell_timer = 2.0f; // cooldown to avoid immediate re-hits

      std::cout << "Koopa enters shell mode and sliding!" << std::endl;
      break;
    }
    case KoopaState::Shell: {
      return;
    }
    case KoopaState::Sliding: {
      alive = false;
      state       = EnemyState::Dead;
      velocity = {0.0f, 0.0f};
      std::cout << "Sliding Koopa destroyed!" << std::endl;
      break;
    }
    default: {
      std::cout << "Koopa in unknown state on hit from above!" << std::endl;
      break;
    }
  }
}

void KoopaTroopa::OnHitFromSide() {
  switch (GetKoopaState()) {
    case KoopaState::Walking: {
      return;
    }
    case KoopaState::Shell: {
      return;
    }
    case KoopaState::Sliding: {
      KickShell(facing_left);
      break;
    }
  }
}

EnemyType KoopaTroopa::GetType() {
  return EnemyType::Koopa;
}

Enemy *KoopaTroopa::Clone() const {
  KoopaTroopa *clone = new KoopaTroopa(*this);
  // Clone the movement strategy
  if (movement_strategy_) {
    switch (koopa_state) {
      case KoopaState::Walking:
        clone->SetMovementStrategy(new WalkingMovement(5.0f));
        break;
      case KoopaState::Sliding:
        clone->SetMovementStrategy(new SlidingMovement(facing_left));
        break;
      case KoopaState::Shell:
        clone->SetMovementStrategy(nullptr);
        break;
    }
  }
  return clone;
}

KoopaState KoopaTroopa::GetKoopaState() const {
  return koopa_state;
}

void KoopaTroopa::KickShell(bool kick_left) {
  if (koopa_state == KoopaState::Shell) {
    koopa_state = KoopaState::Sliding;
    state       = EnemyState::Normal;
    facing_left      = kick_left;

    SetMovementStrategy(new SlidingMovement(kick_left));

    std::cout << "Koopa shell manually kicked!" << std::endl;
  }
}
