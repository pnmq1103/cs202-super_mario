#include "include/enemies/bowser.hpp"
#include "include/enemies/movement_strategy.hpp"
#include "include/managers/enemy_manager.hpp"

Bowser::Bowser(Vector2 pos, float Nscale)
    : Enemy(pos, Nscale, EnemyType::Bowser), hp(5), fire_timer(0.0f),
      attack_cooldown(3.0f), rage_mode(false) {
  // Set up boss movement pattern
  SetMovementStrategy(new BossMovement(10.0f)); // Use boss-specific movement

  // Boss-specific properties
  health          = 5.0f;
  max_health      = 5.0f;
  detection_range = 250.0f; // Larger detection range for boss

  // Set initial velocity for walking
  velocity.x  = 15.0f; // Start walking right (slower than others)
  facing_left = false;

  std::cout << "Bowser spawned with initial velocity: " << velocity.x
            << std::endl;
}

Bowser::~Bowser() {}

void Bowser::OnHit() {
  // Boss is harder to defeat - takes multiple hits
  DealDamage(1.0f);
  hp--;

  if (hp <= 0) {
    alive    = false;
    state    = EnemyState::Dead;
    velocity = {0.0f, 0.0f}; // Stop movement when dead
  } else {
    // Enter rage mode when low on health
    if (hp <= 2 && !rage_mode) {
      EnterRageMode();
    }

    // Brief stun when hit
    Stun(0.5f); // Shorter stun for boss

    // Visual feedback
    invulnerability_timer = 1.0f; // Longer invulnerability
  }
}

void Bowser::Update() {
  if (!alive)
    return;

  // Update fire timer
  fire_timer += GetFrameTime();

  // Advanced Boss AI
  if (player_position && IsPlayerInRange(detection_range)) {
    // Face the player
    bool should_face_left = (player_position->x < position.x);
    SetFacing(should_face_left);

    // Update velocity direction to match facing
    if (state != EnemyState::Stunned) {
      if (should_face_left && velocity.x > 0) {
        velocity.x
          = -abs(velocity.x); // Make velocity negative when facing left
      } else if (!should_face_left && velocity.x < 0) {
        velocity.x
          = abs(velocity.x); // Make velocity positive when facing right
      }
    }

    // Different attack patterns based on health
    float attack_frequency
      = rage_mode ? 1.5f : attack_cooldown; // Faster attacks in rage mode

    // Attack periodically - shoot fireballs at player
    if (fire_timer >= attack_frequency && state != EnemyState::Stunned) {
      ShootFireball();
      fire_timer = 0.0f;
    }

    // In rage mode, move more aggressively toward player
    if (rage_mode && state != EnemyState::Stunned) {
      float distance_to_player = GetDistanceToPlayer();
      if (distance_to_player > 100.0f) {
        // Move toward player more aggressively
        float move_speed = should_face_left ? -40.0f : 40.0f;
        velocity.x       = move_speed;
      } else {
        // Close enough, focus on attacking
        velocity.x *= 0.5f; // Slow down for better aim
      }
    }
  } else {
    // Normal patrol behavior when player not in range
    if (state != EnemyState::Attacking && state != EnemyState::Stunned) {
      // Resume normal movement
      if (GetMovementStrategy()) {
        // Movement strategy will handle patrolling
      }
    }
  }

  // Reset attack state after short duration
  if (state == EnemyState::Attacking) {
    static float attack_state_timer = 0.0f;
    attack_state_timer             += GetFrameTime();
    if (attack_state_timer >= 0.5f) {
      state              = EnemyState::Normal;
      attack_state_timer = 0.0f;
    }
  }

  // Call base enemy update for physics and animation
  Enemy::Update();
}

void Bowser::ShootFireball() {
  // Get ProjectilePool from EnemyManager (you'll need to implement this)
  // For now, we'll add a visual effect and prepare for projectile integration

  // Calculate fireball spawn position (mouth of Bowser)
  Vector2 spawn_position = {
    position.x + (facing_left ? -20.0f : frame.width * scale + 20.0f),
    position.y + frame.height * scale / 3.0f // Roughly mouth height
  };

  // TODO: Integrate with ProjectilePool to actually shoot fireball
  // pool->ShootEnemyFireball(spawn_position, facing_left);

  // Visual feedback - enter attacking state briefly
  state = EnemyState::Attacking;

  // Sound effect (if available)
  // PlaySound("bowser_fire");

  std::cout << "Bowser shoots fireball at position: " << spawn_position.x
            << ", " << spawn_position.y << std::endl;
}

void Bowser::EnterRageMode() {
  rage_mode       = true;
  attack_cooldown = 1.5f; // Faster attacks

  // Increase movement speed in rage mode
  if (
    auto *boss_movement = dynamic_cast<BossMovement *>(GetMovementStrategy())) {
    // boss_movement->EnterRageMode(); // Implement this in BossMovement
  }

  std::cout << "Bowser enters RAGE MODE!" << std::endl;
}

void Bowser::OnHitFromAbove() {
  try {
    // Bowser takes damage from above but is more resistant
    DealDamage(1.0f);
    hp--;

    if (hp <= 0) {
      alive = false;
      state = EnemyState::Dead;
    } else {
      // Bowser doesn't get stunned easily from above hits
      invulnerability_timer = 0.8f;

      // Enter rage mode if health is low
      if (hp <= 2 && !rage_mode) {
        EnterRageMode();
      }
    }
  } catch (const std::exception &e) {
    std::cerr << "Error in Bowser::OnHitFromAbove(): " << e.what() << std::endl;
    // Fallback: mark as dead
    alive    = false;
    state    = EnemyState::Dead;
    velocity = {0.0f, 0.0f};
  }
}

void Bowser::OnHitFromSide() {
  try {
    // Bowser is very resistant to side attacks
    if (state == EnemyState::Stunned || IsInvulnerable()) {
      return; // No damage if already stunned or invulnerable
    }

    DealDamage(0.5f); // Reduced damage from side
    hp--;

    if (hp <= 0) {
      alive = false;
      state = EnemyState::Dead;
    } else {
      // Brief knockback
      velocity.x            = facing_left ? 50.0f : -50.0f;
      invulnerability_timer = 1.0f;

      // Enter rage mode if health is low
      if (hp <= 2 && !rage_mode) {
        EnterRageMode();
      }
    }
  } catch (const std::exception &e) {
    std::cerr << "Error in Bowser::OnHitFromSide(): " << e.what() << std::endl;
    // Fallback: mark as dead
    alive    = false;
    state    = EnemyState::Dead;
    velocity = {0.0f, 0.0f};
  }
}

EnemyType Bowser::GetType() {
  return EnemyType::Bowser;
}

Enemy *Bowser::Clone() const {
  Bowser *clone = new Bowser(*this);
  // Clone the movement strategy
  if (movement_strategy_) {
    clone->SetMovementStrategy(new BossMovement(10.0f));
  }
  return clone;
}

int Bowser::GetHP() const {
  return hp;
}

bool Bowser::IsInRageMode() const {
  return rage_mode;
}
