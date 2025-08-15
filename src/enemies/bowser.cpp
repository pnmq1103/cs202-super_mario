#include "include/enemies/bowser.hpp"

#include "include/core/constants.hpp"
#include "include/enemies/movement_strategy.hpp"
#include "include/managers/enemy_manager.hpp"

Bowser::Bowser(
  Vector2 pos, float Nscale, Character *Ncharacter, ProjectilePool *Npool)
    : Enemy(pos, Nscale, EnemyType::Bowser) {
  character_ = Ncharacter;
  pool_      = Npool;
  health = max_health = 5;
  shoot_timer_        = 200;
  hit_timer_          = 0;
  position.y         -= scale * 41;
  frame               = frame_list[0];
  velocity.x          = 1;
  throw_duration_ = throw_timer_ = 30;
  SetMovementStrategy(new FollowStrategy(character_));
}

Bowser::~Bowser() {}

EnemyType Bowser::GetType() {
  return EnemyType::Bowser;
}

void Bowser::OnHit() {

  if (hit_timer_ > 0)
    return;
  hit_timer_ = 100;
  --health;
  if (health == 0) {
    alive    = false;
    state    = EnemyState::Dead;
    velocity = {0, -50.0f};
  }
}

void Bowser::OnHitFromAbove() {
  OnHit();
}

void Bowser::OnHitFromSide() {
  OnHit();
}

void Bowser::Update() {
  if (!alive) {
    if (IsRunningDeathAnimation()) {
      velocity.y += 10;
      position.y += velocity.y;
    }
    return;
  }
  --shoot_timer_;
  --hit_timer_;

  if (shoot_timer_ == 0) {
    shoot_timer_ = 200;
    shoot_time_  = time;
  }

  if (health <= 2) {
    --throw_timer_;
    if (throw_timer_ == 0) {
      throw_timer_ = throw_duration_;
      pool_->ShootStaticFireball(
        {character_rectangle_.x, character_rectangle_.y});
    } else if (throw_timer_ == 15) {
      character_rectangle_ = character_->GetRectangle();
    }
  }

  if (time - shoot_time_ < 75) {
    Rectangle rect = character_->GetRectangle();
    if (rect.x < position.x) {
      SetFacing(true);
    } else
      SetFacing(false);

    velocity.y += 10;
    position.y += 10;
    frame       = frame_list[(time - shoot_time_) / 15 % 5 + 11];
    if (time - shoot_time_ == 74) {
      Vector2 pos = position;
      pos.y      += 50.f;
      pool_->ShootEnemyFireball(pos, facing_left);
    }
  } else {
    if (time - shoot_time_ == 75) {
      if (character_->GetRectangle().x < position.x) {
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
    }
    SetFacing(facing_left);

    if (CheckCollisionRecs(GetRectangle(), character_->GetRectangle()))
      frame = frame_list[3];
    else
      frame = frame_list[(time / 20) % 3 + 3];
    Enemy::Update();
  }

  if (position.x < 0)
    position.x = 0;
  if (
    position.x + frame.width * scale
    > constants::mapWidth * constants::blockSize)
    position.x
      = constants::mapWidth * constants::blockSize - frame.width * scale;
}

void Bowser::Draw() {

  if (!texture)
    return;

  Color tint = WHITE;
  if (health <= 2)
    tint = RED;

  // Handle sprite flipping for direction - FIXED LOGIC
  Rectangle source_rect = frame;

  // Fix sprite direction: flip based on facing direction AND velocity
  bool shouldFlipSprite = false;
  shouldFlipSprite      = facing_left;

  if (!shouldFlipSprite) {
    source_rect.width = -fabs(source_rect.width); // Flip horizontally
  } else {
    source_rect.width = fabs(source_rect.width); // Normal direction
  }
  // Additional safety check for MakeDestRect
  Rectangle dest_rect = MakeDestRect(frame);

  DrawTexturePro(*texture, source_rect, dest_rect, {0, 0}, 0.0f, tint);
}

Enemy *Bowser::Clone() const {
  return nullptr;
}

bool Bowser::IsRunningDeathAnimation() const {
  if (IsAlive())
    return false;
  else {
    if (position.y > constants::mapHeight * constants::blockSize)
      return false;
    else
      return true;
  }
}

bool Bowser::IsCrazy() {
  if (health <= 2)
    return true;
  else
    return false;
}

void Bowser::ReverseDirection() {
  velocity.x     = 0;
  stop_direction = facing_left;
}

bool Bowser::GetStopDirection() {
  return stop_direction;
}