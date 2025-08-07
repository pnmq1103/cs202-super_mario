#include "include/enemies/movement_strategy.hpp"
#include "include/enemies/enemy.hpp"
#include <algorithm>
#include <cmath>

// WalkingMovement Implementation
WalkingMovement::WalkingMovement(float speed, bool movingLeft)
    : speed_(speed), movingLeft_(movingLeft) {}

void WalkingMovement::Update(Enemy *enemy, float deltaTime) {
  if (!enemy)
    return;

  Vector2 position = enemy->GetPosition();
  Vector2 velocity = enemy->GetVelocity();

  // Horizontal movement
  float direction = movingLeft_ ? -1.0f : 1.0f;
  velocity.x      = speed_ * direction;

  // Apply gravity for ground physics
  velocity.y += 10.f; // Gravity

  // Update position
  position.x += velocity.x;
  position.y += velocity.y;

  // Simple ground collision (assuming ground at y=600)

  enemy->SetPosition(position);
  enemy->SetVelocity(velocity);
}

MovementStrategy *WalkingMovement::Clone() const {
  return new WalkingMovement(speed_, movingLeft_);
}

void WalkingMovement::ReverseDirection() {
  movingLeft_ = !movingLeft_;
}

// JumpingMovement Implementation
JumpingMovement::JumpingMovement(
  float horizontalSpeed, float jumpForce, float jumpInterval)
    : horizontalSpeed_(horizontalSpeed), jumpForce_(jumpForce),
      jumpInterval_(jumpInterval), jumpTimer_(0.0f), verticalVelocity_(0.0f),
      onGround_(true), movingLeft_(true), gravity_(400.0f) {}

void JumpingMovement::Update(Enemy *enemy, float deltaTime) {
  if (!enemy)
    return;

  Vector2 position = enemy->GetPosition();
  Vector2 velocity = enemy->GetVelocity();

  // Horizontal movement
  float direction = movingLeft_ ? -1.0f : 1.0f;
  velocity.x      = horizontalSpeed_ * direction;

  // Jumping logic
  jumpTimer_ += deltaTime;
  if (jumpTimer_ >= jumpInterval_ && onGround_) {
    // Initiate jump
    verticalVelocity_ = -jumpForce_;
    onGround_         = false;
    jumpTimer_        = 0.0f;
  }

  // Apply gravity
  if (!onGround_) {
    verticalVelocity_ += gravity_;
  }

  velocity.y = verticalVelocity_;

  // Update position
  position.x += velocity.x;
  position.y += velocity.y;

  // Ground collision
  if (position.y >= 600.0f) {
    position.y        = 600.0f;
    onGround_         = true;
    verticalVelocity_ = 0;
    velocity.y        = 0;
  }

  enemy->SetPosition(position);
  enemy->SetVelocity(velocity);
}

MovementStrategy *JumpingMovement::Clone() const {
  return new JumpingMovement(horizontalSpeed_, jumpForce_, jumpInterval_);
}

void JumpingMovement::ReverseDirection() {
  movingLeft_ = !movingLeft_;
}

// BossMovement Implementation
BossMovement::BossMovement(float baseSpeed)
    : currentPhase_(BossPhase::WALKING), phaseTimer_(0.0f), speed_(baseSpeed),
      movingLeft_(true), fireTimer_(0.0f), chargeSpeed_(baseSpeed * 2.0f),
      verticalVelocity_(0.0f), onGround_(true), targetPosition_({0, 0}),
      attackCount_(0) {}

void BossMovement::Update(Enemy *enemy, float deltaTime) {
  if (!enemy)
    return;

  phaseTimer_ += deltaTime;

  switch (currentPhase_) {
    case BossPhase::WALKING:
      UpdateWalkingPhase(enemy, deltaTime);
      if (phaseTimer_ >= 3.0f)
        SwitchPhase();
      break;
    case BossPhase::CHARGING:
      UpdateChargingPhase(enemy, deltaTime);
      if (phaseTimer_ >= 2.0f)
        SwitchPhase();
      break;
    case BossPhase::BREATHING_FIRE:
      UpdateFireBreathingPhase(enemy, deltaTime);
      if (phaseTimer_ >= 1.5f)
        SwitchPhase();
      break;
    case BossPhase::JUMPING:
      UpdateJumpingPhase(enemy, deltaTime);
      if (phaseTimer_ >= 2.5f)
        SwitchPhase();
      break;
    case BossPhase::RAGE_MODE:
      UpdateRageMode(enemy, deltaTime);
      if (phaseTimer_ >= 4.0f)
        SwitchPhase();
      break;
  }
}

void BossMovement::UpdateWalkingPhase(Enemy *enemy, float deltaTime) {
  Vector2 position = enemy->GetPosition();
  Vector2 velocity = enemy->GetVelocity();

  float direction = movingLeft_ ? -1.0f : 1.0f;
  velocity.x      = speed_ * direction;

  // Apply gravity
  velocity.y += 10.f;

  // Update position
  position.x += velocity.x;
  position.y += velocity.y;

  // Boundary checking and direction reversal
  if (position.x <= 100.0f || position.x >= 700.0f) {
    movingLeft_ = !movingLeft_;
  }

  enemy->SetPosition(position);
  enemy->SetVelocity(velocity);
}

void BossMovement::UpdateChargingPhase(Enemy *enemy, float deltaTime) {
  Vector2 position = enemy->GetPosition();
  Vector2 velocity = enemy->GetVelocity();

  float direction = movingLeft_ ? -1.0f : 1.0f;
  velocity.x      = chargeSpeed_ * direction;

  // Apply gravity
  velocity.y += 10.f;

  // Update position
  position.x += velocity.x;
  position.y += velocity.y;

  enemy->SetPosition(position);
  enemy->SetVelocity(velocity);
}

void BossMovement::UpdateFireBreathingPhase(Enemy *enemy, float deltaTime) {
  Vector2 position = enemy->GetPosition();
  Vector2 velocity = {0, 0}; // Stay still while breathing fire

  // Apply gravity
  velocity.y += 10.f;
  position.y += velocity.y;

  // Fire breathing effect
  fireTimer_ += deltaTime;
  if (fireTimer_ >= 0.3f) {
    // Could trigger fire projectile creation here
    fireTimer_ = 0.0f;
    attackCount_++;
  }

  enemy->SetPosition(position);
  enemy->SetVelocity(velocity);
}

void BossMovement::UpdateJumpingPhase(Enemy *enemy, float deltaTime) {
  Vector2 position = enemy->GetPosition();
  Vector2 velocity = enemy->GetVelocity();

  // Initiate jump if on ground
  if (onGround_ && phaseTimer_ < 0.1f) {
    verticalVelocity_ = -250.0f; // Strong jump
    onGround_         = false;
  }

  // Apply gravity
  verticalVelocity_ += 10.f;
  velocity.y         = verticalVelocity_;

  // Slight horizontal movement during jump
  float direction = movingLeft_ ? -1.0f : 1.0f;
  velocity.x      = speed_ * 0.5f * direction;

  // Update position
  position.x += velocity.x;
  position.y += velocity.y;

  enemy->SetPosition(position);
  enemy->SetVelocity(velocity);
}

void BossMovement::UpdateRageMode(Enemy *enemy, float deltaTime) {
  Vector2 position = enemy->GetPosition();
  Vector2 velocity = enemy->GetVelocity();

  // Faster, more aggressive movement
  float direction = movingLeft_ ? -1.0f : 1.0f;
  velocity.x      = chargeSpeed_ * 1.5f * direction;

  // Apply gravity
  velocity.y += 10.f;

  // Update position
  position.x += velocity.x;
  position.y += velocity.y;

  // More frequent direction changes in rage mode
  if (position.x <= 50.0f || position.x >= 750.0f) {
    movingLeft_ = !movingLeft_;
  }

  // Ground collision
  if (position.y >= 600.0f) {
    position.y = 600.0f;
    velocity.y = 0;
  }

  enemy->SetPosition(position);
  enemy->SetVelocity(velocity);
}

void BossMovement::SwitchPhase() {
  phaseTimer_  = 0.0f;
  attackCount_ = 0;

  // Cycle through phases
  int nextPhase = (static_cast<int>(currentPhase_) + 1) % 4;
  currentPhase_ = static_cast<BossPhase>(nextPhase);

  // Skip rage mode unless boss is damaged
  if (currentPhase_ == BossPhase::RAGE_MODE) {
    // Only enter rage mode under certain conditions
    // For now, skip to walking
    currentPhase_ = BossPhase::WALKING;
  }
}

void BossMovement::EnterRageMode() {
  currentPhase_ = BossPhase::RAGE_MODE;
  phaseTimer_   = 0.0f;
  chargeSpeed_ *= 1.5f; // Increase charge speed
}

MovementStrategy *BossMovement::Clone() const {
  return new BossMovement(speed_);
}

// StationaryMovement Implementation
StationaryMovement::StationaryMovement(float bobAmount, float bobSpeed)
    : bobTimer_(0.0f), bobAmount_(bobAmount), originalY_(0.0f),
      bobSpeed_(bobSpeed), isEmerging_(false), emergenceTimer_(0.0f) {}

void StationaryMovement::Update(Enemy *enemy, float deltaTime) {
  if (!enemy)
    return;

  Vector2 position = enemy->GetPosition();

  if (originalY_ == 0.0f) {
    originalY_ = position.y;
  }

  bobTimer_ += deltaTime;

  if (isEmerging_) {
    emergenceTimer_ += deltaTime;
    // Piranha plant emergence animation
    float emergenceHeight = std::min(emergenceTimer_ * 50.0f, 32.0f);
    position.y            = originalY_ - emergenceHeight;

    if (emergenceTimer_ >= 2.0f) {
      isEmerging_     = false;
      emergenceTimer_ = 0.0f;
    }
  } else {
    // Simple bobbing motion
    position.y = originalY_ + sin(bobTimer_ * bobSpeed_) * bobAmount_;
  }

  enemy->SetPosition(position);
}

MovementStrategy *StationaryMovement::Clone() const {
  return new StationaryMovement(bobAmount_, bobSpeed_);
}

// SlidingMovement Implementation
SlidingMovement::SlidingMovement(float speed, bool movingLeft, float friction)
    : speed_(speed), movingLeft_(movingLeft), friction_(friction),
      minSpeed_(5.0f), isBouncing_(false) {}

void SlidingMovement::Update(Enemy *enemy, float deltaTime) {
  if (!enemy)
    return;

  Vector2 position = enemy->GetPosition();
  Vector2 velocity = enemy->GetVelocity();

  float direction = movingLeft_ ? -1.0f : 1.0f;
  velocity.x      = speed_ * direction;

  // Apply gravity
  velocity.y += 10.f;

  // Update position
  position.x += velocity.x;
  position.y += velocity.y;

  // Apply friction
  speed_ *= friction_;

  // Stop if speed is too low
  if (speed_ < minSpeed_) {
    speed_     = 0.0f;
    velocity.x = 0;
  }

  enemy->SetPosition(position);
  enemy->SetVelocity(velocity);
}

MovementStrategy *SlidingMovement::Clone() const {
  return new SlidingMovement(speed_, movingLeft_, friction_);
}

void SlidingMovement::ReverseDirection() {
  movingLeft_ = !movingLeft_;
  // Boost speed when bouncing off walls
  if (speed_ < 50.0f) {
    speed_ = 80.0f;
  }
}

// PatrolMovement Implementation
PatrolMovement::PatrolMovement(Vector2 pointA, Vector2 pointB, float speed)
    : pointA_(pointA), pointB_(pointB), speed_(speed), tolerance_(5.0f),
      movingToB_(true) {
  currentTarget_ = pointB_;
}

void PatrolMovement::Update(Enemy *enemy, float deltaTime) {
  if (!enemy)
    return;

  Vector2 position = enemy->GetPosition();
  Vector2 velocity = enemy->GetVelocity();

  // Calculate direction to target
  Vector2 direction
    = {currentTarget_.x - position.x, currentTarget_.y - position.y};
  float distance = sqrt(direction.x * direction.x + direction.y * direction.y);

  if (distance > tolerance_) {
    // Normalize direction
    direction.x /= distance;
    direction.y /= distance;

    // Move towards target
    velocity.x = direction.x * speed_;
    velocity.y = direction.y * speed_;
  } else {
    // Reached target, switch to other point
    if (movingToB_) {
      currentTarget_ = pointA_;
    } else {
      currentTarget_ = pointB_;
    }
    movingToB_ = !movingToB_;
    velocity.x = 0;
    velocity.y = 0;
  }

  // Apply gravity if not flying enemy
  velocity.y += 10.f;

  // Update position
  position.x += velocity.x;
  position.y += velocity.y;

  enemy->SetPosition(position);
  enemy->SetVelocity(velocity);
}

MovementStrategy *PatrolMovement::Clone() const {
  return new PatrolMovement(pointA_, pointB_, speed_);
}

// FollowingMovement Implementation
FollowingMovement::FollowingMovement(
  Vector2 *target, float speed, float detectionRange)
    : targetPosition_(target), speed_(speed), detectionRange_(detectionRange),
      minDistance_(20.0f), isFollowing_(false) {}

void FollowingMovement::Update(Enemy *enemy, float deltaTime) {
  if (!enemy || !targetPosition_)
    return;

  Vector2 position = enemy->GetPosition();
  Vector2 velocity = enemy->GetVelocity();

  // Calculate distance to target
  Vector2 direction
    = {targetPosition_->x - position.x, targetPosition_->y - position.y};
  float distance = sqrt(direction.x * direction.x + direction.y * direction.y);

  // Check if target is in detection range
  if (distance <= detectionRange_ && distance > minDistance_) {
    isFollowing_ = true;

    // Normalize direction
    direction.x /= distance;
    direction.y /= distance;

    // Move towards target
    velocity.x = direction.x * speed_;
    // Only follow horizontally for ground enemies
    // velocity.y = direction.y * speed_; // Uncomment for flying enemies
  } else if (distance <= minDistance_) {
    // Too close, stop following
    isFollowing_ = false;
    velocity.x   = 0;
  } else {
    // Target out of range
    isFollowing_ = false;
    velocity.x   = 0;
  }

  // Apply gravity
  velocity.y += 10.f;

  // Update position
  position.x += velocity.x;
  position.y += velocity.y;

  enemy->SetPosition(position);
  enemy->SetVelocity(velocity);
}

MovementStrategy *FollowingMovement::Clone() const {
  return new FollowingMovement(targetPosition_, speed_, detectionRange_);
}