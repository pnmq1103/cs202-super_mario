#include "include/enemies/enemy.hpp"
#include "include/core/application.hpp"
#include "include/enemies/bowser.hpp"
#include "include/enemies/movement_strategy.hpp"
#include "include/managers/enemy_manager.hpp"
#include <cmath>
#include <fstream>
#include <iostream>
#include <raylib.h>

// Static member initialization following GameObject pattern
int Enemy::enemy_count_ = 0;
int Enemy::time         = 0;

Rectangle Enemy::MakeDestRect(Rectangle rect) {
  return {position.x, position.y, rect.width * scale, rect.height * scale};
}

void Enemy::LoadFrameList(std::string address) {
  std::ifstream fin;
  fin.open(address);
  if (!fin.is_open())
    throw std::runtime_error("Cannot open enemy sprite file: " + address);

  frame_list.clear();
  while (!fin.eof()) {
    Rectangle rect;
    float n;
    if (!(fin >> n))
      break; // Index (not used)
    if (!(fin >> rect.x >> rect.y >> rect.width >> rect.height))
      break;
    frame_list.push_back(rect);
  }
  fin.close();
}

void Enemy::SetFrameCount() {
  ++time;
}

void Enemy::Reset() {
  time         = 0;
  enemy_count_ = 0;
}

Enemy::Enemy(Vector2 Nposition, float Nscale, EnemyType enemy_type)
    : scale(Nscale), gravity(5.0f), type(enemy_type) {
  index_ = enemy_count_;
  ++enemy_count_;

  position = Nposition;
  alive    = true;
  state    = EnemyState::Normal;
  time     = 0;
  velocity = {0, 0};

  // Set ground level based on enemy type
  float ground_level = 500.0f; // Default ground level
  // Set ground reference

  // Enemy-specific initialization
  health                = 1.0f;
  max_health            = 1.0f;
  stun_timer            = 0.0f;
  invulnerability_timer = 0.0f;
  facing_left           = false;
  detection_range       = 100.0f;
  movement_strategy_    = nullptr;
  player_position       = nullptr;

  // Initialize frame with safe default before loading sprites
  frame = {0, 0, 32, 32};

  // Set appropriate texture based on enemy type
  switch (type) {
    case EnemyType::Bowser:
      texture = &App.Resource().GetEnemy('b');
      try {
        LoadFrameList("res/sprites/enemies/bowser.txt");
      } catch (const std::exception &e) {
        std::cerr << "Failed to load Bowser sprites: " << e.what() << std::endl;
      }
      break;
    case EnemyType::Goomba:
    case EnemyType::Koopa:
    case EnemyType::Piranha:
    default:
      texture = &App.Resource().GetEnemy('m');
      try {
        LoadFrameList("res/sprites/enemies/minions.txt");
      } catch (const std::exception &e) {
        std::cerr << "Failed to load minion sprites: " << e.what() << std::endl;
      }
      break;
  }

  // Initialize frame after loading frame list
  if (!frame_list.empty()) {
    UpdateAnimationFrame();
  }
}

Enemy::~Enemy() {
  if (movement_strategy_) {
    delete movement_strategy_;
    movement_strategy_ = nullptr;
  }
}

Rectangle Enemy::GetRectangle() const {
  if (position.x < 0 || position.y < 0) {
    throw std::runtime_error("Invalid position for enemy rectangle");
  }
  // Frame seems valid, use it
  return {position.x, position.y, frame.width * scale, frame.height * scale};
}

Rectangle Enemy::GetRect() const {
    return GetRectangle();
}

bool Enemy::IsAlive() const {
  return alive;
}

void Enemy::Update() {
  if (!alive)
    return;
  try {
    // Update timers
    if (stun_timer > 0.0f) {
      stun_timer -= GetFrameTime();
      if (stun_timer <= 0.0f) {
        state = EnemyState::Normal;
      }
    }

    if (invulnerability_timer > 0.0f) {
      invulnerability_timer -= GetFrameTime();
    }

    // Update movement strategy if not stunned (with safe null check)
    if (alive && state != EnemyState::Stunned && movement_strategy_) {
      movement_strategy_->Update(this, GetFrameTime());
    }

    // ===== ENHANCED WALL COLLISION DETECTION =====
    // Check for wall collision before collision_handler removes enemies
    // CheckAndHandleWallCollision();

    // Update animation frame based on enemy type and state
    UpdateAnimationFrame();

  } catch (const std::exception &e) {
    std::cerr << "Error in Enemy::Update(): " << e.what() << std::endl;
    // Fallback: mark as dead to prevent further crashes
    alive    = false;
    state    = EnemyState::Dead;
    velocity = {0.0f, 0.0f};
  }
}

void Enemy::UpdateAnimationFrame() {
  if (frame_list.empty()) {
    // Set default frame if no frame list loaded
    frame = {0, 0, 32, 32}; // Default 32x32 sprite as per minions.txt
    return;
  }

  int frame_index = 0;

  // Frame selection logic based on exact sprite mapping provided
  switch (type) {
    case EnemyType::Goomba:
      // Goomba: ID 0-4 (5 frames for walking animation)
      if (state == EnemyState::Dead)
        frame_index = 4; // Use last frame for dead state
      else
        frame_index = (time / 8) % 4; // Use all 4 frames for walking animation
      break;

    case EnemyType::Koopa:
      if (state == EnemyState::Shell) {
        // Shell state: use ID 11-16
        frame_index = 11 + ((time / 5) % 6); // Shell animation
      } else {
        // Normal Koopa walking - use only ID 9-10 for both directions
        frame_index = 9 + ((time / 10) % 2);
      }
      break;
    case EnemyType::Piranha:
      // Piranha Plant: ID 5-8 (4 frames)
      frame_index = 5 + ((time / 15) % 4); // Slower animation for piranha
      break;

    case EnemyType::Bowser:
      if (state == EnemyState::Attacking) {
        // Preparing to shoot: ID 11-15
        frame_index = 11 + ((time / 5) % 5); // Attack preparation animation
      } else {
        // Normal movement based on facing_left instead of velocity
        if (!facing_left) {
          // Facing right: ID 9-10
          frame_index = 9 + ((time / 20) % 2);
        } else if (facing_left) {
          // Facing left: ID 0-6
          frame_index = (time / 20) % 7;
        } else {
          // Default/idle: ID 7-8
          frame_index = 7 + ((time / 25) % 2);
        }
      }
      break;
  }

  // Ensure frame_index is within bounds with extra safety
  if (frame_index < 0)
    frame_index = 0;
  if (frame_index >= static_cast<int>(frame_list.size())) {
    frame_index = static_cast<int>(frame_list.size()) - 1;
  }

  // Additional bounds check before accessing frame_list
  if (frame_index >= 0 && frame_index < static_cast<int>(frame_list.size())) {
    Rectangle selected_frame = frame_list[frame_index];

    // Validate frame dimensions before assigning
    if (
      selected_frame.width > 0 && selected_frame.height > 0
      && selected_frame.width <= 1000 && selected_frame.height <= 1000) {
      frame = selected_frame;
    } else {
      // Invalid frame detected - use first frame as fallback
      if (
        !frame_list.empty() && frame_list[0].width > 0
        && frame_list[0].height > 0 && frame_list[0].width <= 1000
        && frame_list[0].height <= 1000) {
        frame = frame_list[0];
      } else {
        // Last resort default frame
        frame = {0, 0, 32, 32};
      }
    }
  } else {
    // Fallback to first frame or default
    if (
      !frame_list.empty() && frame_list[0].width > 0 && frame_list[0].height > 0
      && frame_list[0].width <= 1000 && frame_list[0].height <= 1000) {
      frame = frame_list[0];
    } else {
      frame = {0, 0, 32, 32};
    }
  }
}

void Enemy::Draw() {
  if (!texture)
    return;

  if (!alive && state == EnemyState::Dead) {
    // draw frame for dead enemy
    Rectangle dest_rect = MakeDestRect(frame);
    if (dest_rect.width <= 0 || dest_rect.height <= 0) {
      // Skip drawing if destination rectangle is invalid
      return;
    }
    // Draw dead enemy with a faded tint
    DrawTexturePro(
      *texture, frame, dest_rect, {0, 0}, 0.0f, ColorAlpha(WHITE, 0.5f));
    return;
  }
  // Apply visual effects based on state
  Color tint = WHITE;
  if (state == EnemyState::Stunned) {
    tint = ColorAlpha(WHITE, 0.7f);
  } else if (IsInvulnerable()) {
    tint = ColorAlpha(WHITE, 0.5f);
  }

  // Handle sprite flipping for direction - FIXED LOGIC
  Rectangle source_rect = frame;

  // Fix sprite direction: flip based on facing direction AND velocity
  bool shouldFlipSprite = false;

  switch (type) {
    case EnemyType::Goomba:
      shouldFlipSprite = (velocity.x < 0); // Flip when moving left
      break;
    case EnemyType::Koopa:
      shouldFlipSprite = (velocity.x < 0); // Flip when moving left
      break;
    case EnemyType::Bowser:
      // These enemies should face the direction they're moving
      // If moving left (velocity.x < 0), show left-facing sprite
      // If moving right (velocity.x > 0), show right-facing sprite
      shouldFlipSprite = (velocity.x < 0); // Flip when moving left
      break;
    case EnemyType::Piranha:
      // Piranha doesn't move horizontally, use facing_left only
      shouldFlipSprite = facing_left;
      break;
  }

  if (!shouldFlipSprite) {
    source_rect.width = -fabs(source_rect.width); // Flip horizontally
  } else {
    source_rect.width = fabs(source_rect.width); // Normal direction
  }
  // Additional safety check for MakeDestRect
  Rectangle dest_rect = MakeDestRect(frame);
  if (dest_rect.width <= 0 || dest_rect.height <= 0) {
    // Skip drawing if destination rectangle is invalid
    return;
  }

  DrawTexturePro(*texture, source_rect, dest_rect, {0, 0}, 0.0f, tint);
}

void Enemy::Bounce() {
  velocity.y = -5.0f;
}

void Enemy::ReverseDirection() {
  // Store old direction for logging
  bool was_facing_left = facing_left;
  float old_velocity   = velocity.x;

  // Reverse direction
  velocity.x = -velocity.x;

  // Update facing direction to match new velocity - FIXED LOGIC
  if (velocity.x > 0) {
    facing_left = false; // Moving right, face right
  } else if (velocity.x < 0) {
    facing_left = true; // Moving left, face left
  }

  movement_strategy_->ReverseDirection();
  // If velocity.x == 0, keep current facing direction

  // Debug logging

  // Update animation frame immediately to reflect direction change
  UpdateAnimationFrame();
}

void Enemy::SetMovementStrategy(MovementStrategy *strategy) {
  if (movement_strategy_) {
    delete movement_strategy_;
    movement_strategy_ = nullptr; // Ensure pointer is nullified after deletion
  }
  movement_strategy_ = strategy;
}

void Enemy::UpdateMovement(float dt) {
  // Added safe null check before using movement_strategy_
  if (movement_strategy_ && state != EnemyState::Stunned && alive) {
    movement_strategy_->Update(this, dt);
  }
}

MovementStrategy *Enemy::GetMovementStrategy() const {
  return movement_strategy_;
}

void Enemy::StopY(float Ny) {
  position.y = Ny - frame.height * scale;
  velocity.y = 0;
}

void Enemy::HandleGroundCollision(float ground_level) {}

void Enemy::HandleWallCollision() {
  // Simple wall collision - reverse direction
  ReverseDirection();
}

bool Enemy::CanAttack() const {
  return state == EnemyState::Normal && invulnerability_timer <= 0.0f;
}

void Enemy::EnterAttackMode() {
  if (state != EnemyState::Stunned) {
    state = EnemyState::Attacking;
  }
}

void Enemy::PerformAttack() {
  // Default attack behavior - can be overridden by subclasses
  // For now, just stay in attack mode briefly
  state = EnemyState::Attacking;
}

void Enemy::OnProjectileHit(Vector2 projectile_pos, int damage) {
  (void)projectile_pos; // Suppress unused parameter warning
  try {
    DealDamage(static_cast<float>(damage));
  } catch (const std::exception &e) {
    std::cerr << "Error in Enemy::OnProjectileHit(): " << e.what() << std::endl;
    // Fallback: mark as dead
    alive = false;
    state = EnemyState::Dead;
  }
}

// Safe wrapper methods for virtual functions
void Enemy::SafeOnHitFromAbove() {
  try {
    OnHitFromAbove();
  } catch (const std::exception &e) {
    std::cerr << "Error in OnHitFromAbove(): " << e.what() << std::endl;
    // Fallback: just mark as dead
    alive = false;
    state = EnemyState::Dead;
  }
}

void Enemy::SafeOnHitFromSide() {
  try {
    OnHitFromSide();
  } catch (const std::exception &e) {
    std::cerr << "Error in OnHitFromSide(): " << e.what() << std::endl;
    // Fallback: just mark as dead
    alive = false;
    state = EnemyState::Dead;
  }
}

// Enhanced collision detection with proper direction checking
bool Enemy::IsHitFromAbove(const Rectangle &characterRect) const {
  Rectangle enemyRect = GetRectangle();

  // Calculate collision overlap
  float overlapX
    = std::min(
        characterRect.x + characterRect.width, enemyRect.x + enemyRect.width)
      - std::max(characterRect.x, enemyRect.x);
  float overlapY
    = std::min(
        characterRect.y + characterRect.height, enemyRect.y + enemyRect.height)
      - std::max(characterRect.y, enemyRect.y);

  // Character is hitting from above if:
  // 1. Character's bottom is close to enemy's top
  // 2. Most of the overlap is vertical
  float charBottom = characterRect.y + characterRect.height;
  float enemyTop   = enemyRect.y;

  return (charBottom <= enemyTop + enemyRect.height * 0.3f)
         && (overlapY < overlapX);
}

// Enhanced wall collision detection
void Enemy::CheckAndHandleWallCollision() {
  // Get current rectangle for collision checking
  Rectangle rect = GetRectangle();

  // Define map boundaries (should match collision_handler boundaries)
  float leftWall  = 0.0f;
  float rightWall = 3200.0f; // Default map width - can be made configurable

  // Check if enemy is approaching or touching walls
  bool shouldReverseLeft  = false;
  bool shouldReverseRight = false;

  // Left wall collision detection
  if (rect.x <= leftWall + 5.0f && velocity.x < 0) {
    shouldReverseLeft = true;
  }

  // Right wall collision detection
  if (rect.x + rect.width >= rightWall - 5.0f && velocity.x > 0) {
    shouldReverseRight = true;
  }

  // Handle wall collision based on enemy type
  if (shouldReverseLeft || shouldReverseRight) {
    switch (type) {
      case EnemyType::Goomba:
      case EnemyType::Koopa:
        // Normal enemies turn around when hitting walls

        ReverseDirection();

        // Adjust position to prevent getting stuck in wall
        if (shouldReverseLeft) {
          position.x = leftWall + 1.0f;
        } else if (shouldReverseRight) {
          position.x = rightWall - rect.width - 1.0f;
        }

        break;

      case EnemyType::Piranha:
        // Piranha plants don't move horizontally, so no wall collision
        break;

      case EnemyType::Bowser:
        // Bowser also reverses direction at walls

        ReverseDirection();

        if (shouldReverseLeft) {
          position.x = leftWall + 1.0f;
        } else if (shouldReverseRight) {
          position.x = rightWall - rect.width - 1.0f;
        }

        break;
    }
  }
}

// Getters and setters
Vector2 Enemy::GetPosition() const {
  return position;
}

Vector2 Enemy::GetVelocity() const {
  return velocity;
}

EnemyState Enemy::GetState() const {
  return state;
}

float Enemy::GetHealth() const {
  return health;
}

float Enemy::GetMaxHealth() const {
  return max_health;
}

void Enemy::SetPosition(Vector2 pos) {
  position = pos;
}

void Enemy::SetVelocity(Vector2 vel) {
  velocity = vel;
}

void Enemy::SetPlayerReference(Vector2 *player_pos) {
  player_position = player_pos;
}

// Utility methods
float Enemy::GetDistanceToPlayer() const {
  if (!player_position)
    return 9999.0f;

  float dx = player_position->x - position.x;
  float dy = player_position->y - position.y;
  return sqrtf(dx * dx + dy * dy);
}

bool Enemy::IsPlayerInRange(float range) const {
  return GetDistanceToPlayer() <= range;
}

void Enemy::Stun(float duration) {
  state      = EnemyState::Stunned;
  stun_timer = duration;
  velocity.x = 0.0f; // Stop horizontal movement
}

void Enemy::DealDamage(float damage) {
  if (IsInvulnerable())
    return;

  try {
    health -= damage;
    if (health <= 0.0f) {
      health = 0.0f;
      alive  = false;
      state  = EnemyState::Dead;
      MarkForDeletion();
      EnemyManager::GetInstance().OnEnemyDeath(index_);
      // Debug log

    } else {
      // Become invulnerable briefly after taking damage
      invulnerability_timer = 0.5f;

      // Debug log
    }
  } catch (const std::exception &e) {
    std::cerr << "Error in Enemy::DealDamage(): " << e.what() << std::endl;
    // Fallback: mark as dead
    alive = false;
    state = EnemyState::Dead;
    MarkForDeletion();
  }
}

bool Enemy::IsInvulnerable() const {
  return invulnerability_timer > 0.0f;
}

bool Enemy::IsFacingLeft() const {
  return facing_left;
}

void Enemy::SetFacing(bool left) {
  facing_left = left;
}

// Legacy compatibility methods - DEPRECATED
// Use CollisionHandler instead of these methods
bool Enemy::CheckCollision(Rectangle other) const {
  Rectangle my_rect = GetRectangle();
  return CheckCollisionRecs(my_rect, other);
}

void Enemy::Render(
  Texture &tex, const std::unordered_map<int, Rectangle> &sprite_rects) const {
  // Legacy method - use Draw() instead
  const_cast<Enemy *>(this)->Draw();
}
