#include "include/enemies/piranha_plant.hpp"
#include "include/enemies/movement_strategy.hpp"
#include <iostream>

PiranhaPlant::PiranhaPlant(Vector2 pos, float Nscale)
    : Enemy(pos, Nscale, EnemyType::Piranha), is_emerging(false),
      emerge_timer(0.0f) {

  // Set piranha plant properties
  health     = 2.0f;
  max_health = 2.0f;

  // Make sure texture and animations are properly initialized
  if (frame_list.size() >= 9) { // Make sure we have enough frames
    // Set initial frame to piranha plant (frames 5-8 in minions.txt)
    frame = frame_list[5];
  } else {
    // Fallback if frame list is not properly loaded
    std::cerr << "Warning: Piranha Plant frame list not properly loaded\n";
  }

  // Debug output to verify
  std::cout << "Piranha Plant created at position: " << pos.x << ", " << pos.y
            << std::endl;
  std::cout << "Frame list size: " << frame_list.size() << std::endl;

  // CRITICAL: Set the proper movement strategy with visible initial position
  // Make sure the plant is initially visible above the pipe
  PiranhaMovementStrategy *strategy = new PiranhaMovementStrategy();
  strategy->SetEmergeDistance(45.0f * scale); // Scale properly with sprite size
  SetMovementStrategy(strategy);
}

PiranhaPlant::~PiranhaPlant() {
  // Base class will clean up movement_strategy_
}

void PiranhaPlant::Update() {
  if (!alive)
    return;

  // Let the base class update handle movement strategy execution
  Enemy::Update();

  // Ensure proper animation frames are selected
  // Override the base class animation if needed
  int frame_index
    = 5 + ((time / 8) % 4); // Faster animation (changed from 15 to 8)
  if (frame_index >= 0 && frame_index < static_cast<int>(frame_list.size())) {
    frame = frame_list[frame_index];
  }
}

void PiranhaPlant::OnHit() {
  // Piranha plants only take damage from projectiles
  // The actual damage handling happens in OnProjectileHit in the base class
}

void PiranhaPlant::OnHitFromAbove() {
  // Piranha plants can't be killed by jumping on them
  // Player should take damage instead
  // Make sure to play the "hit" sound
  App.Media().PlaySound("bump");
}

void PiranhaPlant::OnHitFromSide() {
  // Piranha plants can't be killed from the side either
  // Player should take damage instead
  // Make sure to play the "hit" sound
  App.Media().PlaySound("bump");
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

  // If we have a PiranhaMovementStrategy, tell it to start emerging
  if (movement_strategy_) {
    PiranhaMovementStrategy *strategy
      = dynamic_cast<PiranhaMovementStrategy *>(movement_strategy_);
    if (strategy) {
      strategy->StartEmerging();
    }
  }
}

void PiranhaPlant::StartHiding() {
  is_emerging  = false;
  emerge_timer = 0.0f;

  // If we have a PiranhaMovementStrategy, tell it to start hiding
  if (movement_strategy_) {
    PiranhaMovementStrategy *strategy
      = dynamic_cast<PiranhaMovementStrategy *>(movement_strategy_);
    if (strategy) {
      strategy->StartHiding();
    }
  }
}

bool PiranhaPlant::IsEmerging() const {
  return is_emerging;
}

void PiranhaPlant::ForceEmerge() {
  is_emerging = true;

  // Force the movement strategy to fully emerge
  if (movement_strategy_) {
    PiranhaMovementStrategy *strategy
      = dynamic_cast<PiranhaMovementStrategy *>(movement_strategy_);
    if (strategy) {
      strategy->ForceEmerge();
    }
  }
}

void PiranhaPlant::ForceHide() {
  is_emerging = false;

  // Force the movement strategy to fully hide
  if (movement_strategy_) {
    PiranhaMovementStrategy *strategy
      = dynamic_cast<PiranhaMovementStrategy *>(movement_strategy_);
    if (strategy) {
      strategy->ForceHide();
    }
  }
}