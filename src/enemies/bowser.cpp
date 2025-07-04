#include "../include/enemies/bowser.hpp"

Bowser::Bowser(Vector2 pos, int spriteId)
    : Enemy(pos, 32, 32, EnemyType::Bowser, spriteId), hp(5) {}

void Bowser::update(float dt) {
  if (!alive)
    return;
  fireTimer += dt;

  if (fireTimer > 3.0f) {
    fireTimer = 0;
  }
}

void Bowser::onHitFromAbove() {
  hp--;
  if (hp <= 0)
    alive = false;
}

void Bowser::onHitFromSide() {
  hp--;
  if (hp <= 0)
    alive = false;
}

Enemy *Bowser::clone() const {
  return new Bowser(*this);
}

int Bowser::getHP() const {
  return hp;
}
