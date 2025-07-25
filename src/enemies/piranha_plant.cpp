#include "include/enemies/piranha_plant.hpp"

PiranhaPlant::PiranhaPlant(Vector2 pos, Vector2 size, Vector2 velo, int spriteId)
    : Enemy(EnemyType::Piranha, pos, size, velo, spriteId) {}

void PiranhaPlant::Update(float dt) {
  if (!alive)
    return;

  time += dt;
  if (time >= interval) {
    up   = !up;
    time = 0;
  }

  position.y += (up ? -10 : 10) * dt;
}

void PiranhaPlant::OnHitFromAbove() {}

void PiranhaPlant::OnHitFromSide() {
  alive = false;
}

Enemy *PiranhaPlant::Clone() const {
  return new PiranhaPlant(*this);
}
