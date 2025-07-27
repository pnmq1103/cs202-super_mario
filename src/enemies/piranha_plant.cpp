#include "include/enemies/piranha_plant.hpp"
#include "include/enemies/movement_strategy.hpp"

PiranhaPlant::PiranhaPlant(Vector2 pos, int spriteId)
    : Enemy(pos, 16, 24, EnemyType::Piranha, spriteId) {
  // Set up stationary movement with bobbing for plants in pipes
  SetMovementStrategy(new StationaryMovement(8.0f)); // Bob amount: 8 pixels
}

void PiranhaPlant::Update(float dt) {
  if (!alive)
    return;
    
  time += dt;
  
  // Custom piranha plant behavior - emerging from pipes
  if (time >= interval) {
    up = !up;
    time = 0;
  }
  
  // Use stationary movement strategy for bobbing effect
  Enemy::Update(dt);
  
  // Additional piranha-specific movement (emerging/hiding)
  Vector2 currentPos = GetPosition();
  if (up) {
    // Emerge from pipe
    currentPos.y -= 20.0f * dt;
  } else {
    // Hide in pipe
    currentPos.y += 20.0f * dt;
  }
  
  // Clamp position to pipe boundaries
  float maxY = 600.0f; // Ground level
  float minY = 580.0f; // Top of pipe
  currentPos.y = currentPos.y < minY ? minY : (currentPos.y > maxY ? maxY : currentPos.y);
  
  SetPosition(currentPos);
}

void PiranhaPlant::OnHitFromAbove() {
  // Piranha plants are usually invulnerable to stomping
  // They hurt Mario instead
  // alive = false; // Uncomment if you want them to be killable
}

void PiranhaPlant::OnHitFromSide() {
  // Can be killed by fireballs or other projectiles
  alive = false;
}

Enemy *PiranhaPlant::Clone() const {
  PiranhaPlant* clone = new PiranhaPlant(*this);
  // Clone the movement strategy
  if (movementStrategy_) {
    clone->SetMovementStrategy(movementStrategy_->Clone());
  }
  return clone;
}
