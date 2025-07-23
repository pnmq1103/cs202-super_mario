#include "include/characters/fireball.hpp"

//+----------------------------------------------------------+
//|                       Mario Fireball                     |
//+----------------------------------------------------------+
MarioFireball::MarioFireball() : Projectile(5, {10, 0}) {
  gravity_            = 5;
  bounce_coefficient_ = 1;
  range_              = 1000;

  LoadFrameList("res/sprites/icons/objects.txt");
  Image image = LoadImage("res/sprites/icons/objects.png");
  ImageResize(&image, image.width * scale, image.height * scale);
  texture = LoadTextureFromImage(image);
  UnloadImage(image);
  frame = frame_list[133];
}
MarioFireball::~MarioFireball() {}
void MarioFireball::Update() {
  if (is_destroy)
    return;
  frame = frame_list[(time / 15) % 3 + 133];

  if (velocity.x > 0) {
    if (position.x >= original_position.x + range_)
      is_destroy = true;
  } else {
    if (position.x <= original_position.x - range_)
      is_destroy = true;
  }
  velocity.y += gravity_;
  position.x += velocity.x;
  position.y += velocity.y;
}
void MarioFireball::Draw() {
  if (is_destroy)
    return;
  Projectile::Draw();
}
void MarioFireball::StopY(bool stop_upper, bool stop_lower) {
  if (stop_upper) {
    velocity.y = 0;
  }
  if (stop_lower)
    velocity.y *= -bounce_coefficient_;
}

ProjectileType MarioFireball::GetType() {
  return mario_fireball;
}
//+----------------------------------------------------------+
//|                       Enemy Fireball                     |
//+----------------------------------------------------------+
EnemyFireball::EnemyFireball() : Projectile(5, {10, 0}) {
  LoadFrameList("res/sprites/icons/objects.txt");
  Image image = LoadImage("res/sprites/icons/objects.png");
  ImageResize(&image, image.width * scale, image.height * scale);
  texture = LoadTextureFromImage(image);
  UnloadImage(image);
  frame  = frame_list[133];
  range_ = 1000;
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

  if ((time / 5) % 3 == 0) {
    frame = frame_list[745];
  } else if ((time / 5) % 3 == 1) {
    frame = frame_list[759];
  } else {
    frame = frame_list[773];
  }
  position.x += velocity.x;
  position.y += velocity.y;
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
//+----------------------------------------------------------+
//|                       Electric ball                      |
//+----------------------------------------------------------+
ElectricBall::ElectricBall() : Projectile(1, {20, 0}) {

  LoadFrameList("res/sprites/electric_shot/electric_shot.txt");
  Image image = LoadImage("res/sprites/electric_shot/electric_shot.png");
  ImageResize(&image, image.width * scale, image.height * scale);
  texture = LoadTextureFromImage(image);
  UnloadImage(image);
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
      if (!is_explode_)
        Destroy();
  } else {
    if (position.x <= original_position.x - range_)
      if (!is_explode_)
        Destroy();
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