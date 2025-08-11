#pragma once
#include "include/characters/character.hpp"
#include "include/core/constants.hpp"
#include <memory>
#include <raylib.h>
#include <unordered_map>

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
  void Update(Enemy *enemy, float dt) override;
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
  SlidingMovement(bool movingLeft = true);
  void Update(Enemy *enemy, float deltaTime) override;
  MovementStrategy *Clone() const override;

  void ReverseDirection() override;
};

class FollowStrategy : public MovementStrategy {
private:
  Character *character_;

public:
  FollowStrategy(Character *Ncharacter);
  ~FollowStrategy();
  void Update(Enemy *enemy, float deltaTime) override;
  MovementStrategy *Clone() const override;
};
