#pragma once
#include <memory>
#include <raylib.h>
#include <unordered_map>
#include "include/core/constants.hpp"

class Enemy; // Forward declaration
class PiranhaPlant;
// Strategy interface for enemy movement
class MovementStrategy {
public:
  virtual ~MovementStrategy()                        = default;
  virtual void Update(Enemy *enemy, float deltaTime) = 0;
  virtual MovementStrategy *Clone() const            = 0;
  virtual void ReverseDirection() {} // Optional override
};

class PiranhaMovementStrategy : public MovementStrategy {
private:
  float emergeTime     = 2.0f;  // Time to stay emerged (seconds)
  float hideTime       = 2.0f;  // Time to stay hidden (seconds)
  float emergeDistance = 32.0f; // Distance to emerge (pixels)
  float timer          = 0.0f;  // Current state timer
  bool isEmerging
    = true; // Whether currently emerging (vs hiding) - start as emerging
  float initialY = 0.0f; // Initial Y position (fully hidden)
  float targetY  = 0.0f; // Target Y position (fully emerged)

public:
  PiranhaMovementStrategy();
  void Update (Enemy *enemy, float dt) override;
  void ReverseDirection() override;
  MovementStrategy *Clone() const override;

  // Add these methods to control emergence/hiding
  void SetEmergeDistance(float distance) {
    emergeDistance = distance;
  }
  void StartEmerging() {
    isEmerging = true;
    timer      = 0.0f;
  }
  void StartHiding() {
    isEmerging = false;
    timer      = 0.0f;
  }
  void ForceEmerge();
  void ForceHide();
};
// Basic walking movement - left and right
class WalkingMovement : public MovementStrategy {
private:
  float speed_;
  bool movingLeft_;

public:
  WalkingMovement(float speed = 50.0f, bool movingLeft = true);
  void Update(Enemy *enemy, float deltaTime) override;
  MovementStrategy *Clone() const override;

  void ReverseDirection() override;
  float GetSpeed() const {
    return speed_;
  }
  bool IsMovingLeft() const {
    return movingLeft_;
  }
  void SetSpeed(float speed) {
    speed_ = speed;
  }
};

// Jumping movement - periodic jumps with physics
class JumpingMovement : public MovementStrategy {
private:
  float horizontalSpeed_;
  float jumpForce_;
  float jumpTimer_;
  float jumpInterval_;
  float verticalVelocity_;
  bool onGround_;
  bool movingLeft_;
  float gravity_;

public:
  JumpingMovement(
    float horizontalSpeed = 30.0f, float jumpForce = 200.0f,
    float jumpInterval = 2.0f);
  void Update(Enemy *enemy, float deltaTime) override;
  MovementStrategy *Clone() const override;

  void ReverseDirection() override;
  bool IsOnGround() const {
    return onGround_;
  }
  void ForceJump() {
    if (onGround_) {
      verticalVelocity_ = -jumpForce_;
      onGround_         = false;
    }
  }
};

// Boss movement - complex pattern for Bowser with multiple phases
class BossMovement : public MovementStrategy {
private:
  enum class BossPhase {
    WALKING,
    CHARGING,
    BREATHING_FIRE,
    JUMPING,
    RAGE_MODE
  };

  BossPhase currentPhase_;
  float phaseTimer_;
  float speed_;
  bool movingLeft_;
  float fireTimer_;
  float chargeSpeed_;
  float verticalVelocity_;
  bool onGround_;
  Vector2 targetPosition_;
  int attackCount_;

public:
  BossMovement(float baseSpeed = 40.0f);
  void Update(Enemy *enemy, float deltaTime) override;
  MovementStrategy *Clone() const override;

  void EnterRageMode();
  BossPhase GetCurrentPhase() const {
    return currentPhase_;
  }

private:
  void UpdateWalkingPhase(Enemy *enemy, float deltaTime);
  void UpdateChargingPhase(Enemy *enemy, float deltaTime);
  void UpdateFireBreathingPhase(Enemy *enemy, float deltaTime);
  void UpdateJumpingPhase(Enemy *enemy, float deltaTime);
  void UpdateRageMode(Enemy *enemy, float deltaTime);
  void SwitchPhase();
  void ReverseDirection() {
    speed_ = -speed_;
  }
};

// Stationary movement - for plants in pipes with bobbing
class StationaryMovement : public MovementStrategy {
private:
  float bobTimer_;
  float bobAmount_;
  float originalY_;
  float bobSpeed_;
  bool isEmerging_;
  float emergenceTimer_;

public:
  StationaryMovement(float bobAmount = 5.0f, float bobSpeed = 2.0f);
  void Update(Enemy *enemy, float deltaTime) override;
  MovementStrategy *Clone() const override;

  void StartEmergence() {
    isEmerging_     = true;
    emergenceTimer_ = 0.0f;
  }
  bool IsEmerging() const {
    return isEmerging_;
  }
};

// Sliding movement - for Koopa shells with physics
class SlidingMovement : public MovementStrategy {
private:
  bool movingLeft_;
  float minSpeed_;
  bool isBouncing_;

public:
  SlidingMovement(
 bool movingLeft = true);
  void Update(Enemy *enemy, float deltaTime) override;
  MovementStrategy *Clone() const override;

  void ReverseDirection() override;

};

// Patrol movement - moves between two points
class PatrolMovement : public MovementStrategy {
private:
  Vector2 pointA_;
  Vector2 pointB_;
  Vector2 currentTarget_;
  float speed_;
  float tolerance_;
  bool movingToB_;

public:
  PatrolMovement(Vector2 pointA, Vector2 pointB, float speed = 40.0f);
  void Update(Enemy *enemy, float deltaTime) override;
  MovementStrategy *Clone() const override;

  void ReverseDirection() override {
    movingToB_ = !movingToB_;
  }
};

// Following movement - follows a target (like Mario)
class FollowingMovement : public MovementStrategy {
private:
  Vector2 *targetPosition_;
  float speed_;
  float detectionRange_;
  float minDistance_;
  bool isFollowing_;

public:
  FollowingMovement(
    Vector2 *target, float speed = 60.0f, float detectionRange = 100.0f);
  void Update(Enemy *enemy, float deltaTime) override;
  MovementStrategy *Clone() const override;

  void SetTarget(Vector2 *target) {
    targetPosition_ = target;
  }
  bool IsFollowing() const {
    return isFollowing_;
  }
  float GetDetectionRange() const {
    return detectionRange_;
  }
};