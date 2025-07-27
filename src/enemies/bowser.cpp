#include "include/enemies/bowser.hpp"
#include "include/enemies/movement_strategy.hpp"

Bowser::Bowser(Vector2 pos, int spriteId)
    : Enemy(pos, 48, 48, EnemyType::Bowser, spriteId), hp(5) {
  // Set up boss movement strategy for complex behavior
  SetMovementStrategy(new BossMovement(40.0f)); // Boss-specific movement
}

void Bowser::Update(float dt) {
  if (!alive)
    return;
    
  // Update fire timer for special attacks
  fireTimer += dt;
  
  // Call base class update which handles strategy
  Enemy::Update(dt);
  
  // Boss-specific update logic
  if (fireTimer > 3.0f) {
    // Could trigger special fire attack here
    fireTimer = 0;
    // This could spawn fire projectiles or change behavior
    
    // Change movement pattern based on health for dynamic boss fight
    if (hp <= 2) {
      // Switch to more aggressive movement when low health
      BossMovement* bossStrategy = dynamic_cast<BossMovement*>(movementStrategy_);
      if (bossStrategy) {
        // Boss becomes more aggressive at low health
        // Could modify strategy parameters here
      }
    }
  }
}

void Bowser::OnHitFromAbove() {
  hp--;
  
  // Boss special reaction to being hit from above
  if (hp > 0) {
    // Flash or show damage indicator
    // Could add invincibility frames here
    
    // Become more aggressive when hit
    BossMovement* bossStrategy = dynamic_cast<BossMovement*>(movementStrategy_);
    if (bossStrategy) {
      // Could force a phase change or special attack
      fireTimer = 2.9f; // Trigger attack soon
    }
  } else {
    alive = false;
    // Boss death sequence could be added here
  }
}

void Bowser::OnHitFromSide() {
  hp--;
  
  if (hp > 0) {
    // Boss reacts differently to side hits
    ReverseDirection(); // Change direction when hit
    
    // Trigger special behavior
    fireTimer = 2.5f; // Trigger attack even sooner when hit from side
  } else {
    alive = false;
    // Boss death sequence
  }
}

Enemy *Bowser::Clone() const {
  Bowser* clone = new Bowser(*this);
  // Clone the movement strategy
  if (movementStrategy_) {
    clone->SetMovementStrategy(movementStrategy_->Clone());
  }
  return clone;
}

int Bowser::getHP() const {
  return hp;
}

bool Bowser::IsVulnerable() const {
  // Boss might have invincibility frames after being hit
  return hp > 0;
}

void Bowser::EnterRageMode() {
  // Special method to make boss more dangerous
  if (hp <= 2) {
    // Could change movement strategy to more aggressive one
    SetMovementStrategy(new BossMovement(60.0f)); // Faster movement
    fireTimer = 0; // Reset fire timer for immediate attack
  }
}

void Bowser::PerformSpecialAttack() {
  // Boss special attack - could spawn multiple projectiles
  // This would integrate with projectile system
  fireTimer = 0; // Reset timer
}

Vector2 Bowser::GetCenterPosition() const {
  Vector2 pos = GetPosition();
  return {pos.x + width/2.0f, pos.y + height/2.0f};
}
