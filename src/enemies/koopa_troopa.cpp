#include "../include/enemies/koopa_troopa.hpp"

KoopaTroopa::KoopaTroopa(Vector2 pos, int spriteId)
    : Enemy(pos, 16, 16, EnemyType::Koopa, spriteId),
      state(KoopaState::Walking) {
  velocity.x = -20;
}

void KoopaTroopa::update(float dt) {
  if (!alive)
    return;

  switch (state) {
    case KoopaState::Walking:
      position.x += velocity.x * dt;
      break;
    case KoopaState::Shell:
      shellTimer += dt;
      if (shellTimer > 5.0f) {
        state      = KoopaState::Walking;
        velocity.x = -20;
        shellTimer = 0;
      }
      break;
    case KoopaState::Sliding:
      position.x += velocity.x * dt * 3;
      break;
  }
}

void KoopaTroopa::onHitFromAbove() {
  if (state == KoopaState::Walking) {
    state      = KoopaState::Shell;
    velocity.x = 0;
  } else if (state == KoopaState::Shell) {
    state      = KoopaState::Sliding;
    velocity.x = 60;
  } else {
    state      = KoopaState::Shell;
    velocity.x = 0;
  }
}

void KoopaTroopa::onHitFromSide() {
  alive = false;
}

Enemy *KoopaTroopa::clone() const {
  return new KoopaTroopa(*this);
}

KoopaState KoopaTroopa::getState() const {
  return state;
}
