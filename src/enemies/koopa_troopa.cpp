#include "include/enemies/koopa_troopa.hpp"

KoopaTroopa::KoopaTroopa(Vector2 pos, Vector2 size, Vector2 velo, int spriteId)
    : Enemy(EnemyType::Koopa, pos, size, velo, spriteId),
      state(KoopaState::Walking) {
  velocity.x = -20;
}

void KoopaTroopa::Update(float dt) {
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

void KoopaTroopa::OnHitFromAbove() {
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

void KoopaTroopa::OnHitFromSide() {
  alive = false;
}

Enemy *KoopaTroopa::Clone() const {
  return new KoopaTroopa(*this);
}

KoopaState KoopaTroopa::GetState() const {
  return state;
}
