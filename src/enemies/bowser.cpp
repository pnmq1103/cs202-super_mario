#include "include/enemies/bowser.hpp"

Bowser::Bowser(
  Vector2 pos, Vector2 size, Vector2 velo, int spriteId)
    : Enemy(EnemyType::Bowser, pos, size, velo, spriteId), hp(5) {}

void Bowser::Update(float dt) {
  if (!alive)
    return;
  fireTimer += dt;

  if (fireTimer > 3.0f) {
    fireTimer = 0;
  }
}

void Bowser::OnHitFromAbove() {
  hp--;
  if (hp <= 0)
    alive = false;
}

void Bowser::OnHitFromSide() {
  hp--;
  if (hp <= 0)
    alive = false;
}

Enemy *Bowser::Clone() const {
  return new Bowser(*this);
}

int Bowser::getHP() const {
  return hp;
}
