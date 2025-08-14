#include "include/enemies/koopa_troopa.hpp"
#include "include/core/constants.hpp"
#include "include/enemies/movement_strategy.hpp"
#include <iostream>

KoopaTroopa::KoopaTroopa(Vector2 pos, float Nscale)
    : Enemy(pos, Nscale, EnemyType::Koopa), koopa_state(KoopaState::Walking),
      shell_timer(0.0f) {
  SetMovementStrategy(new WalkingMovement(5.0f));

  health          = 1.0f;
  max_health      = 1.0f;
  detection_range = 100.0f;

  velocity.x  = -10.0f;
  facing_left = true;

  position.y -= 27 * scale;

  std::cout << "Koopa spawned with initial velocity: " << velocity.x
            << std::endl;
}

KoopaTroopa::~KoopaTroopa() {}

void KoopaTroopa::OnHit() {
  // Generic hit (e.g. projectile) kills Koopa directly
  if (!IsAlive())
    return;
  KillAndNotify();
  std::cout << "Koopa defeated by direct hit! +200 points\n";
}

void KoopaTroopa::Update() {
  if (!IsAlive()) {
    if (IsRunningDeathAnimation()) {
      velocity.y += 10;
      position.y += velocity.y;
      position.x += velocity.x;
    }
    return;
  }

  if (shell_timer > 0.0f) {
    shell_timer -= GetFrameTime();
  }

  Enemy::Update();
}

void KoopaTroopa::OnHitFromAbove() {
  if (!IsAlive())
    return;
  if (shell_timer > 0.0f) {
    return;
  }

  switch (GetKoopaState()) {
    case KoopaState::Walking: {
      koopa_state = KoopaState::Sliding; // Keep existing behavior
      state       = EnemyState::Shell;
      SetMovementStrategy(new SlidingMovement()); // Starts moving immediately
      shell_timer = 2.5f;
      std::cout << "Koopa stomped -> shell sliding!\n";
      break;
    }
    case KoopaState::Shell: {
      // Optional: could kick shell here instead of ignoring
      return;
    }
    case KoopaState::Sliding: {
      KillAndNotify();
      std::cout << "Sliding Koopa destroyed! +200 points\n";
      break;
    }
  }
}

void KoopaTroopa::OnHitFromSide() {
  if (!IsAlive())
    return;
  if (shell_timer > 0.0f) {
    return;
  }

  switch (GetKoopaState()) {
    case KoopaState::Walking: {
      koopa_state = KoopaState::Sliding;
      state       = EnemyState::Shell;
      SetMovementStrategy(new SlidingMovement());
      shell_timer = 2.0f;
      std::cout << "Koopa enters shell sliding (side)!\n";
      break;
    }
    case KoopaState::Shell: {
      // Could call KickShell(...) here if you want side hit to start sliding
      return;
    }
    case KoopaState::Sliding: {
      KillAndNotify();
      std::cout << "Sliding Koopa destroyed from side! +200 points\n";
      break;
    }
  }
}

EnemyType KoopaTroopa::GetType() {
  return EnemyType::Koopa;
}

Enemy *KoopaTroopa::Clone() const {
  KoopaTroopa *clone = new KoopaTroopa(*this);
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
    facing_left = kick_left;
    SetMovementStrategy(new SlidingMovement(kick_left));
    std::cout << "Koopa shell kicked!\n";
  }
}

bool KoopaTroopa::IsRunningDeathAnimation() const {
  if (IsAlive())
    return false;
  if (position.y > constants::mapHeight * constants::blockSize)
    return false;
  return true;
}