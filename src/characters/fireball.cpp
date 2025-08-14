#include "include/characters/fireball.hpp"
#include "include/core/constants.hpp"
#include <cmath>

//+----------------------------------------------------------+
//|                       Mario Fireball                     |
//+----------------------------------------------------------+
MarioFireball::MarioFireball() : Projectile(5, {20, 0}) {
  gravity_            = 5;
  bounce_coefficient_ = 0.9f;
  range_              = 1000;

  LoadFrameList("res/sprites/objects/block_objects.txt");
  texture = &App.Resource().GetObject();
  frame   = frame_list[19];
}
MarioFireball::~MarioFireball() {}
void MarioFireball::Update() {
  if (is_destroy)
    return;
  if (time > 100) {
    Destroy();
    return;
  }
  frame = frame_list[(time / 15) % 3 + 19];

  if (velocity.x > 0) {
    if (position.x >= original_position.x + range_) {
      is_destroy = true;
    }

  } else {
    if (position.x <= original_position.x - range_) {
      is_destroy = true;
    }
  }
  position.x += velocity.x;
  position.y += velocity.y;
  velocity.y += gravity_;

  if (position.x + frame.width * scale < 0)
    Destroy();
  if (position.x > constants::mapWidth * constants::blockSize)
    Destroy();
}
void MarioFireball::Draw() {
  if (is_destroy)
    return;
  Projectile::Draw();
}

void MarioFireball::Renew(Vector2 Nposition, bool to_left) {
  velocity = {20, 0};
  Projectile::Renew(Nposition, to_left);
}

void MarioFireball::StopY(bool stop_upper, bool stop_lower) {
  if (stop_upper) {
    velocity.y = 0;
  }
  if (stop_lower)
    velocity.y = -bounce_coefficient_ * fabs(velocity.y);
}

ProjectileType MarioFireball::GetType() {
  return mario_fireball;
}
void MarioFireball::ReverseDirection() {
  velocity.x *= -1;
}
//+----------------------------------------------------------+
//|                       Enemy Fireball                     |
//+----------------------------------------------------------+
EnemyFireball::EnemyFireball() : Projectile(5, {10, 0}) {
  LoadFrameList("res/sprites/enemies/bowser.txt");
  texture = &App.Resource().GetEnemy('b');
  frame   = frame_list[16];
  range_  = 1000;
}
EnemyFireball::~EnemyFireball() {}
void EnemyFireball::Update() {
  if (is_destroy)
    return;

  if (velocity.x > 0) {
    if (position.x >= original_position.x + range_)
      is_destroy = true;
  } else {
    if (position.x <= original_position.x - range_)
      is_destroy = true;
  }

  frame = frame_list[(time / 5) % 2 + 16];

  position.x += velocity.x;
  position.y += velocity.y;

  if (position.x + frame.width * scale < 0)
    Destroy();
  if (position.x > constants::mapWidth * constants::blockSize)
    Destroy();
}
void EnemyFireball::Draw() {
  if (is_destroy)
    return;
  Projectile::Draw();
}
void EnemyFireball::StopY(bool stop_upper, bool stop_lower) {}

ProjectileType EnemyFireball::GetType() {
  return enemy_fireball;
}

void EnemyFireball::ReverseDirection() {}
//+----------------------------------------------------------+
//|                       Electric ball                      |
//+----------------------------------------------------------+
ElectricBall::ElectricBall() : Projectile(1, {20, 0}) {

  LoadFrameList("res/sprites/electric_shot/electric_shot.txt");
  texture     = &App.Resource().GetElectricShot();
  frame       = frame_list[0];
  is_explode_ = false;
  range_      = 1000;
}
ElectricBall::~ElectricBall() {}
void ElectricBall::Update() {
  if (is_destroy)
    return;

  if (velocity.x > 0) {
    if (position.x >= original_position.x + range_)
      if (!is_explode_) {
        Destroy();
      }

  } else {
    if (position.x <= original_position.x - range_)
      if (!is_explode_) {
        Destroy();
      }
  }
  if (is_explode_) {
    if (time - time_explode_ == 40) {
      is_destroy = true;
      return;
    }
    frame = frame_list[(time - time_explode_) / 5];
  } else {
    position.x += velocity.x;
    position.y += velocity.y;
  }

  if (position.x + frame.width * scale < 0)
    Destroy();
  if (position.x > constants::mapWidth * constants::blockSize)
    Destroy();
}
void ElectricBall::Draw() {
  if (is_destroy)
    return;
  Projectile::Draw();
}
void ElectricBall::Destroy() {
  if (is_destroy || is_explode_)
    return;
  is_explode_   = true;
  time_explode_ = time;
}

void ElectricBall::Renew(Vector2 Nposition, bool to_left) {
  Projectile::Renew(Nposition, to_left);
  frame       = frame_list[0];
  is_explode_ = false;
}
void ElectricBall::StopY(bool stop_upper, bool stop_lower) {}

ProjectileType ElectricBall::GetType() {
  return electric_ball;
}

void ElectricBall::ReverseDirection() {}

//+----------------------------------------------------------+
//|                       static fireball                    |
//+----------------------------------------------------------+

StaticFireball::StaticFireball() : Projectile(4, {0, 0}) {
  LoadFrameList("res/sprites/enemies/bowser.txt");
  texture       = &App.Resource().GetEnemy('b');
  frame         = frame_list[18];
  existing_time = 100;
}

StaticFireball::~StaticFireball() {}

void StaticFireball::Update() {
  if (time > existing_time)
    is_destroy = true;
  if (is_destroy)
    return;
  frame = frame_list[(time / 10) % 3 + 18];
}

void StaticFireball::StopY(bool stop_upper, bool stop_lower) {}

ProjectileType StaticFireball::GetType() {
  return enemy_fireball;
}

void StaticFireball::ReverseDirection() {}