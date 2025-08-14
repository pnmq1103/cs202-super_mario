#include "include/enemies/goomba.hpp"
#include "include/enemies/movement_strategy.hpp"
#include "include/managers/enemy_manager.hpp"

Goomba::Goomba(Vector2 pos, float Nscale)
    : Enemy(pos, Nscale, EnemyType::Goomba) {
  SetMovementStrategy(new WalkingMovement(5.0f));

  health          = 1.0f;
  max_health      = 1.0f;
  detection_range = 80.0f;

  velocity.x  = 30.0f;
  facing_left = false;
  position.y -= scale * 16;

  std::cout << "Goomba spawned with initial velocity: " << velocity.x
            << std::endl;
}

Goomba::~Goomba() {}

void Goomba::OnHit() {
  KillAndNotify();
}

void Goomba::Update() {
  if (!IsAlive()) {
    if (IsRunningDeathAnimation()) {
      if (frame_list.size() > 4)
        frame = frame_list[4];
    }
    return;
  }

  if (GetState() == EnemyState::Normal && GetMovementStrategy()) {
    // Movement strategy handles walking
  }

  Enemy::Update();
}

void Goomba::OnHitFromAbove() {
  try {
    KillAndNotify();
    std::cout << "Goomba defeated by stomp! +100 points" << std::endl;
  } catch (const std::exception &e) {
    std::cerr << "Error in Goomba::OnHitFromAbove(): " << e.what() << std::endl;
    KillAndNotify();
  }
}

void Goomba::OnHitFromSide() {
  try {
    KillAndNotify();
    std::cout << "Goomba defeated by projectile! +100 points" << std::endl;
  } catch (const std::exception &e) {
    std::cerr << "Error in Goomba::OnHitFromSide(): " << e.what() << std::endl;
    KillAndNotify();
  }
}

EnemyType Goomba::GetType() {
  return EnemyType::Goomba;
}

Enemy *Goomba::Clone() const {
  Goomba *clone = new Goomba(*this);
  if (movement_strategy_) {
    clone->SetMovementStrategy(new WalkingMovement(5.0f));
  }
  return clone;
}