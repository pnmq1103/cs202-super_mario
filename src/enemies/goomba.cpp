#include "include/enemies/goomba.hpp"
#include "include/enemies/movement_strategy.hpp"
#include "include/managers/enemy_manager.hpp"

Goomba::Goomba(Vector2 pos, float Nscale)
    : Enemy(pos, Nscale, EnemyType::Goomba) {
  // Set up basic walking movement for Goomba
  SetMovementStrategy(new WalkingMovement(5.0f)); // 50 pixels per second

  // Goomba-specific properties
  health          = 1.0f;
  max_health      = 1.0f;
  detection_range = 80.0f;

  // Set initial velocity for walking
  velocity.x  = 30.0f; // Start walking right
  facing_left = false;

  position.y -= scale * 16;

  std::cout << "Goomba spawned with initial velocity: " << velocity.x
            << std::endl;
}

Goomba::~Goomba() {}

void Goomba::OnHit() {
  // Goomba dies immediately when hit (stomped or from side)
  alive      = false;
  state      = EnemyState::Dead;
  velocity   = {0.0f, 0.0f};
  time_death = time;

  // Visual death effect - flatten sprite
  // This could trigger a death animation
}

void Goomba::Update() {
  if (!alive) {
    if (IsRunningDeathAnimation()) {
      frame = frame_list[2];
    }
    return;
  }

  // Simple AI - just walk and reverse direction on walls
  if (state == EnemyState::Normal && GetMovementStrategy()) {
    // Basic walking behavior is handled by movement strategy
  }

  // Call base enemy update (handles physics, animation, movement)
  Enemy::Update();
}

void Goomba::OnHitFromAbove() {
  try {
    // Mario jumped on Goomba - Goomba dies instantly
    OnHit();

    // Create visual effect (squash animation)
    state = EnemyState::Dead;

    // Award points to player (this would be handled by game manager)
    std::cout << "Goomba defeated by stomp! +100 points" << std::endl;

  } catch (const std::exception &e) {
    std::cerr << "Error in Goomba::OnHitFromAbove(): " << e.what() << std::endl;
    // Fallback: just mark as dead
    alive    = false;
    state    = EnemyState::Dead;
    velocity = {0.0f, 0.0f};
  }
}

void Goomba::OnHitFromSide() {
  try {
    // Side collision with Mario (when not star power) - Mario takes damage
    // But this should be handled by collision system, not here
    // Goomba itself doesn't change when Mario hits from side (unless Mario has
    // star power)

    // If projectile hits from side, Goomba dies
    OnHit();
    std::cout << "Goomba defeated by projectile! +100 points" << std::endl;
  } catch (const std::exception &e) {
    std::cerr << "Error in Goomba::OnHitFromSide(): " << e.what() << std::endl;
    // Fallback: just mark as dead
    alive    = false;
    state    = EnemyState::Dead;
    velocity = {0.0f, 0.0f};
  }
}

EnemyType Goomba::GetType() {
  return EnemyType::Goomba;
}

Enemy *Goomba::Clone() const {
  Goomba *clone = new Goomba(*this);
  // Clone the movement strategy
  if (movement_strategy_) {
    clone->SetMovementStrategy(new WalkingMovement(5.0f));
  }
  return clone;
}
