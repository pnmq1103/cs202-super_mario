#include "Goomba.hpp"

Goomba::Goomba(Vector2 pos, int spriteId)
    : Enemy(pos, 16, 16, EnemyType::Goomba, spriteId) {
    velocity.x = -30;
}

void Goomba::update(float dt) {
    if (!alive) return;
    position.x += velocity.x * dt;
}

void Goomba::onHitFromAbove() {
    alive = false;
}

void Goomba::onHitFromSide() {
    alive = false;
}

Enemy* Goomba::clone() const {
    return new Goomba(*this);
}
