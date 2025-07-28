#include "include/enemies/goomba.hpp"
#include "include/enemies/movement_strategy.hpp"

Goomba::Goomba(Vector2 pos, int spriteId)
    : Enemy(pos, 16, 16, EnemyType::Goomba, spriteId) {
  // Set up basic walking movement for Goomba
  SetMovementStrategy(new WalkingMovement(50.0f));  // 50 pixels per second

  // Goomba-specific properties
  health = 1.0f;
  maxHealth = 1.0f;
  detectionRange = 80.0f;
}

void Goomba::Update(float dt) {
  if (!alive)
    return;

  // Call base enemy update
  Enemy::Update(dt);
}

void Goomba::OnHitFromAbove() {
  // Goomba dies when stomped
  alive = false;
  state = EnemyState::Dead;
  velocity = {0.0f, 0.0f};
}

void Goomba::OnHitFromSide() {
  // Goomba dies when hit from side (fireball, etc.)
  alive = false;
  state = EnemyState::Dead;
}

Enemy *Goomba::Clone() const {
  Goomba *clone = new Goomba(*this);
  // Clone the movement strategy
  if (movementStrategy_) {
    clone->SetMovementStrategy(movementStrategy_->Clone());
  }
  return clone;
}
