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

  std::cout << "Koopa spawned with initial velocity: " << velocity.x
            << std::endl;
}

KoopaTroopa::~KoopaTroopa() {}

void KoopaTroopa::OnHit() {
  if (koopa_state == KoopaState::Walking) {
    // First hit: Enter shell mode
    koopa_state = KoopaState::Shell;
    state       = EnemyState::Shell; // Set the base Enemy state
    velocity    = {0.0f, 0.0f};
    shell_timer = 0.0f;

    // Safely remove movement strategy while in shell
    SetMovementStrategy(nullptr);

    std::cout << "Koopa enters shell mode!" << std::endl;

  } else if (koopa_state == KoopaState::Shell) {
    // Second hit while in shell: Kick the shell (sliding mode)
    koopa_state = KoopaState::Sliding;
    state       = EnemyState::Normal; // Resume normal state but sliding

    // Determine kick direction based on player position or random
    float kick_speed = 10.0f;
    velocity.x       = facing_left ? -kick_speed : kick_speed;

    // Set sliding movement strategy
    SetMovementStrategy(new SlidingMovement(kick_speed, facing_left));

    std::cout << "Koopa shell kicked! Sliding at speed: " << velocity.x
              << std::endl;

  } else if (koopa_state == KoopaState::Sliding) {
    // Hit while sliding: Koopa dies
    alive    = false;
    state    = EnemyState::Dead;
    velocity = {0.0f, 0.0f};

    std::cout << "Sliding Koopa destroyed!" << std::endl;
  }
}

void KoopaTroopa::Update() {
  if (!alive)
    return;

  // Update shell timer
  if (koopa_state == KoopaState::Shell) {
    shell_timer += GetFrameTime();
    // After 5 seconds in shell, come back out
    if (shell_timer >= 5.0f) {
      koopa_state = KoopaState::Walking;
      shell_timer = 0.0f;
      state       = EnemyState::Normal;
      SetMovementStrategy(new WalkingMovement(5.0f));

      std::cout << "Koopa exits shell and resumes walking!" << std::endl;
    }
  }

  // Handle sliding collisions with walls
  if (koopa_state == KoopaState::Sliding) {
    // Sliding shell bounces off walls
    if (
      auto *sliding_movement
      = dynamic_cast<SlidingMovement *>(GetMovementStrategy())) {
      if (!sliding_movement->IsMoving()) {
        // Shell stopped sliding, enter shell mode
        koopa_state = KoopaState::Shell;
        state       = EnemyState::Shell;
        shell_timer = 0.0f;
        SetMovementStrategy(nullptr);

        std::cout << "Sliding Koopa stopped, back to shell mode!" << std::endl;
      }
    }
  }

  // Call base enemy update
  Enemy::Update();
}

void KoopaTroopa::OnHitFromAbove() {
  try {
    // Mario jumped on Koopa
    if (koopa_state == KoopaState::Walking) {
      // Enter shell mode
      OnHit();
    } else if (koopa_state == KoopaState::Shell) {
      // Kick the shell
      OnHit();
    } else if (koopa_state == KoopaState::Sliding) {
      // Stop the shell
      velocity.x *= 0.1f; // Drastically reduce speed
      if (
        auto *sliding_movement
        = dynamic_cast<SlidingMovement *>(GetMovementStrategy())) {
        sliding_movement->SetSpeed(10.0f); // Very slow
      }
    }
  } catch (const std::exception &e) {
    std::cerr << "Error in KoopaTroopa::OnHitFromAbove(): " << e.what()
              << std::endl;
    // Fallback: just mark as dead
    alive    = false;
    state    = EnemyState::Dead;
    velocity = {0.0f, 0.0f};
  }
}

void KoopaTroopa::OnHitFromSide() {
  try {
    if (koopa_state == KoopaState::Shell) {
      // Kick the shell from side
      OnHit();
    } else if (koopa_state == KoopaState::Sliding) {
      // Sliding shell can be destroyed by fireballs
      alive    = false;
      state    = EnemyState::Dead;
      velocity = {0.0f, 0.0f};
      std::cout << "Sliding Koopa destroyed by projectile!" << std::endl;
    } else {
      // Walking Koopa hit from side - enter shell
      OnHit();
    }
  } catch (const std::exception &e) {
    std::cerr << "Error in KoopaTroopa::OnHitFromSide(): " << e.what()
              << std::endl;
    // Fallback: just mark as dead
    alive    = false;
    state    = EnemyState::Dead;
    velocity = {0.0f, 0.0f};
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
        clone->SetMovementStrategy(new SlidingMovement(10.0f, facing_left));
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

    float kick_speed = 10.0f;
    velocity.x       = kick_left ? -kick_speed : kick_speed;
    facing_left      = kick_left;

    SetMovementStrategy(new SlidingMovement(kick_speed, kick_left));

    std::cout << "Koopa shell manually kicked!" << std::endl;
  }
}
