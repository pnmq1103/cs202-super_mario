#include "PiranhaPlant.hpp"

PiranhaPlant::PiranhaPlant(Vector2 pos, int spriteId)
    : Enemy(pos, 16, 24, EnemyType::Piranha, spriteId) {}

void PiranhaPlant::update(float dt) {
    if (!alive) return;

    time += dt;
    if (time >= interval) {
        up = !up;
        time = 0;
    }

    position.y += (up ? -10 : 10) * dt;
}

void PiranhaPlant::onHitFromAbove() {
}

void PiranhaPlant::onHitFromSide() {
    alive = false;
}

Enemy* PiranhaPlant::clone() const {
    return new PiranhaPlant(*this);
}
