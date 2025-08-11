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
  if (position.x <= 0.0f) {
    position.x = 0.0f; // Prevent going off-screen
    direction  = 1.0f; // Reverse direction on left edge
    ReverseDirection();
  }
  if (position.x >= constants::blockSize * constants::mapWidth) { // map size
                                                                  // bruh im so
                                                                  // done
    position.x
      = constants::blockSize * constants::mapWidth; // Prevent going off-screen
    direction = -1.0f; // Reverse direction on right edge
    ReverseDirection();
  }

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
SlidingMovement::SlidingMovement(bool movingLeft)
    : movingLeft_(movingLeft), minSpeed_(5.0f), isBouncing_(false) {}

void SlidingMovement::Update(Enemy *enemy, float deltaTime) {
  if (!enemy)
    return;

  Vector2 position = enemy->GetPosition();
  Vector2 velocity = enemy->GetVelocity();

  float direction = movingLeft_ ? -1.0f : 1.0f;
  velocity.x      = 10.0f * direction;

  // Apply gravity
  velocity.y += 10.f;

  // Update position

  position.x += velocity.x;
  position.y += velocity.y;
  if (position.x <= 0.0f) {
    position.x = 0.0f; // Prevent going off-screen
    direction  = 1.0f; // Reverse direction on left edge
    ReverseDirection();
  }
  if (position.x >= constants::blockSize * constants::mapWidth) { // map size
                                                                  // bruh im so
                                                                  // done
    position.x
      = constants::blockSize * constants::mapWidth; // Prevent going off-screen
    direction = -1.0f; // Reverse direction on right edge
    ReverseDirection();
  }
  // remove the friction from the speed

  enemy->SetPosition(position);
  enemy->SetVelocity(velocity);
}

MovementStrategy *SlidingMovement::Clone() const {
  return new SlidingMovement(movingLeft_);
}

void SlidingMovement::ReverseDirection() {
  movingLeft_ = !movingLeft_;
}

PiranhaMovementStrategy::PiranhaMovementStrategy() {
  // Initialize in hiding state so it emerges first
  isEmerging = false;
  timer      = 0.0f;
}

void PiranhaMovementStrategy::Update(Enemy *enemy, float dt) {
  // Get current position
  Vector2 pos = enemy->GetPosition();

  // First time setup - record initial position
  if (initialY == 0.0f) {
    initialY = pos.y;
    targetY  = initialY - emergeDistance;
    std::cout << "Piranha plant initial Y: " << initialY
              << ", target Y: " << targetY << std::endl;
  }

  // Update timer
  timer += dt;

  if (isEmerging) {
    // Calculate how far to emerge (0.0 to 1.0)
    float t = std::min(timer / emergeTime, 1.0f);
    // Move upward (emerge)
    float newY = initialY - (t * emergeDistance);
    enemy->SetPosition({pos.x, newY});

    // Check if fully emerged
    if (timer >= emergeTime) {
      // Start hiding after staying emerged
      isEmerging = false;
      timer      = 0.0f;
    }
  } else {
    // Calculate how far to hide (0.0 to 1.0)
    float t = std::min(timer / hideTime, 1.0f);
    // Move downward (hide)
    float newY = targetY + (t * emergeDistance);
    enemy->SetPosition({pos.x, newY});

    // Check if fully hidden
    if (timer >= hideTime) {
      // Start emerging after staying hidden
      isEmerging = true;
      timer      = 0.0f;
    }
  }
}

void PiranhaMovementStrategy::ReverseDirection() {
  // Piranha plants don't change horizontal direction
}

MovementStrategy *PiranhaMovementStrategy::Clone() const {
  return new PiranhaMovementStrategy(*this);
}
void PiranhaMovementStrategy::ForceEmerge() {
  isEmerging = true;
  timer      = emergeTime; // Set to maximum time to fully emerge immediately
}

void PiranhaMovementStrategy::ForceHide() {
  isEmerging = false;
  timer      = hideTime; // Set to maximum time to fully hide immediately
}

// follow strategy

FollowStrategy::FollowStrategy(Character *Ncharacter) {
  character_ = Ncharacter;
}

FollowStrategy::~FollowStrategy() {}

void FollowStrategy::Update(Enemy *enemy, float deltaTime) {
  Rectangle character_rect = character_->GetRectangle();
  Rectangle enemy_rect     = enemy->GetRectangle();

  Vector2 position = enemy->GetPosition(), velocity = enemy->GetVelocity();

  velocity.y += 10;
  position.y += velocity.y;

  if (
    character_rect.x <= enemy_rect.x + enemy_rect.width
    && character_rect.x + character_rect.width > enemy_rect.x) {
    enemy->SetVelocity(velocity);
    enemy->SetPosition(position);
    return;
  }

  bool facing_left = false;
  if (character_rect.x < enemy_rect.x) {
    if (character_->IsStarman())
      facing_left = false;
    else
      facing_left = true;
  } else {
    if (character_->IsStarman())
      facing_left = true;
    else
      facing_left = false;
  }

  if (velocity.x == 0) {
    if (facing_left != enemy->GetStopDirection()) {
      if (enemy->IsCrazy())
        velocity.x = 10;
      else
        velocity.x = 5;
    } else {
      velocity.x = 0;
    }
  } else {
    if (enemy->IsCrazy())
      velocity.x = 10;
    else
      velocity.x = 5;
  }

  enemy->SetVelocity(velocity);
  enemy->SetFacing(facing_left);

  velocity    = enemy->GetVelocity();
  position.x += velocity.x;

  enemy->SetPosition(position);
}

MovementStrategy *FollowStrategy::Clone() const {
  return nullptr;
}
