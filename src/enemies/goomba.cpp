#include "include/enemies/goomba.hpp"
#include "include/enemies/movement_strategy.hpp"

Goomba::Goomba(Vector2 pos, int spriteId)
    : Enemy(pos, 16, 16, EnemyType::Goomba, spriteId) {
  // Set up walking movement strategy
  SetMovementStrategy(new WalkingMovement(30.0f, true));  // Speed: 30, moving left
}

void Goomba::Update(float dt) {
  if (!alive)
    return;

  // Call base class update which handles strategy
  Enemy::Update(dt);
}

void Goomba::OnHitFromAbove() {
  alive = false;
  // Could add squashing animation or effect here
}

void Goomba::OnHitFromSide() {
  alive = false;
  // Could add different death effect for side hits
}

Enemy *Goomba::Clone() const {
  Goomba *clone = new Goomba(*this);
  // Clone the movement strategy
  if (movementStrategy_) {
    clone->SetMovementStrategy(movementStrategy_->Clone());
  }
  return clone;
}
