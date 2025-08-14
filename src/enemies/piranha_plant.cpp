#include "include/enemies/piranha_plant.hpp"
#include "include/enemies/movement_strategy.hpp"
#include <iostream>

PiranhaPlant::PiranhaPlant(Vector2 pos, float Nscale)
    : Enemy(pos, Nscale, EnemyType::Piranha), is_emerging(false),
      emerge_timer(0.0f) {

  health     = 2.0f;
  max_health = 2.0f;

  if (frame_list.size() >= 9) {
    frame = frame_list[5];
  } else {
    std::cerr << "Warning: Piranha Plant frame list not properly loaded\n";
  }

  std::cout << "Piranha Plant created at position: " << pos.x << ", " << pos.y
            << std::endl;
  std::cout << "Frame list size: " << frame_list.size() << std::endl;

  auto *strategy = new PiranhaMovementStrategy();
  strategy->SetEmergeDistance(45.0f * scale);
  SetMovementStrategy(strategy);
}

PiranhaPlant::~PiranhaPlant() {}

void PiranhaPlant::Update() {
  if (!alive) {
    if (IsRunningDeathAnimation()) {
      velocity.y += 10;
      position.x += velocity.x;
      position.y += velocity.y;
    }
    return;
  }

  Enemy::Update();

  int frame_index = 5 + ((time / 8) % 4);
  if (frame_index >= 0 && frame_index < (int)frame_list.size()) {
    frame = frame_list[frame_index];
  }
}

void PiranhaPlant::OnHit() {
  KillAndNotify();
  velocity = {10.0f, -50.0f}; // optional death motion (will vanish quickly)
}

void PiranhaPlant::OnHitFromAbove() {
  App.Media().PlaySound("bump");
}

void PiranhaPlant::OnHitFromSide() {
  App.Media().PlaySound("bump");
  OnHit();
}

EnemyType PiranhaPlant::GetType() {
  return EnemyType::Piranha;
}

Enemy *PiranhaPlant::Clone() const {
  return new PiranhaPlant(position, scale);
}

void PiranhaPlant::StartEmerging() {
  is_emerging  = true;
  emerge_timer = 0.0f;
  if (movement_strategy_) {
    auto *strategy
      = dynamic_cast<PiranhaMovementStrategy *>(movement_strategy_);
    if (strategy)
      strategy->StartEmerging();
  }
}

void PiranhaPlant::StartHiding() {
  is_emerging  = false;
  emerge_timer = 0.0f;
  if (movement_strategy_) {
    auto *strategy
      = dynamic_cast<PiranhaMovementStrategy *>(movement_strategy_);
    if (strategy)
      strategy->StartHiding();
  }
}

bool PiranhaPlant::IsEmerging() const {
  return is_emerging;
}

void PiranhaPlant::ForceEmerge() {
  is_emerging = true;
  if (movement_strategy_) {
    auto *strategy
      = dynamic_cast<PiranhaMovementStrategy *>(movement_strategy_);
    if (strategy)
      strategy->ForceEmerge();
  }
}

void PiranhaPlant::ForceHide() {
  is_emerging = false;
  if (movement_strategy_) {
    auto *strategy
      = dynamic_cast<PiranhaMovementStrategy *>(movement_strategy_);
    if (strategy)
      strategy->ForceHide();
  }
}

bool PiranhaPlant::IsRunningDeathAnimation() const {
  if (IsAlive())
    return false;
  if (position.y > constants::mapHeight * constants::blockSize)
    return false;
  return true;
}