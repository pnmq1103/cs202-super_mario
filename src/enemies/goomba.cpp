#include "include/enemies/goomba.hpp"

Goomba::Goomba(Vector2 pos, int spriteId)
    : Enemy(pos, 16, 16, EnemyType::Goomba, spriteId) {
  velocity.x = -30;
}

void Goomba::Update(float dt) {
  if (!alive)
    return;
  position.x += velocity.x * dt;
}

void Goomba::OnHitFromAbove() {
  alive = false;
}

void Goomba::OnHitFromSide() {
  alive = false;
}

Enemy *Goomba::Clone() const {
  return new Goomba(*this);
}
