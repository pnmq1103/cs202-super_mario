#include "include/characters/fireball.hpp"

//+----------------------------------------------------------+
//|                       Mario Fireball                     |
//+----------------------------------------------------------+
MarioFireball::MarioFireball(Vector2 Nposition, bool to_left)
    : Projectile(5, Nposition, {10, 0}) {
  gravity_            = 5;
  bounce_coefficient_ = 1;
  if (to_left)
    velocity.x *= -1;
  range_ = 3000;

  LoadFrameList("res/sprites/icons/objects.txt");
  Image image = LoadImage("res/sprites/icons/objects.png");
  ImageResize(&image, image.width * scale, image.height * scale);
  texture = LoadTextureFromImage(image);
  UnloadImage(image);
  frame = frame_list[133];
}
MarioFireball::~MarioFireball() {}
void MarioFireball::Update(float upper_bounce, float lower_bounce) {
  frame = frame_list[(time / 15) % 3 + 133];
  if (velocity.x > 0) {
    if (position.x >= original_position.x + range_)
      is_destroy = true;
  } else {
    if (position.x <= original_position.x - range_)
      is_destroy = true;
  }
  if (position.y <= upper_bounce) {
    velocity.y  = 0;
    velocity.y += gravity_;
  } else if (position.y + frame.height >= lower_bounce)
    velocity.y *= -bounce_coefficient_;
  else
    velocity.y += gravity_;
  position.x += velocity.x;
  position.y += velocity.y;
}
void MarioFireball::Draw() {
  if (!is_destroy)
    Projectile::Draw();
}
//+----------------------------------------------------------+
//|                       Enemy Fireball                     |
//+----------------------------------------------------------+
EnemyFireball::EnemyFireball(Vector2 Nposition, bool to_left)
    : Projectile(5, Nposition, {10, 0}) {
  if (to_left)
    velocity.x *= -1;

  LoadFrameList("res/sprites/icons/objects.txt");
  Image image = LoadImage("res/sprites/icons/objects.png");
  ImageResize(&image, image.width * scale, image.height * scale);
  texture = LoadTextureFromImage(image);
  UnloadImage(image);
  frame = frame_list[133];
}
EnemyFireball::~EnemyFireball() {}
void EnemyFireball::Update() {
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
  if (!is_destroy)
    Projectile::Draw();
}
//+----------------------------------------------------------+
//|                       Electric ball                      |
//+----------------------------------------------------------+
ElectricBall::ElectricBall(Vector2 Nposition, bool to_left)
    : Projectile(1, Nposition, {10, 0}) {
  if (to_left)
    velocity.x *= -1;

  LoadFrameList("res/sprites/electric_shot/electric_shot.txt");
  Image image = LoadImage("res/sprites/electric_shot/electric_shot.png");
  ImageResize(&image, image.width * scale, image.height * scale);
  texture = LoadTextureFromImage(image);
  UnloadImage(image);
  frame = frame_list[0];
}
ElectricBall::~ElectricBall() {}
void ElectricBall::Update() {
  if (is_destroy) {
    frame = frame_list[(time - time_explode_) / 5];
  } else {
    position.x += velocity.x;
    position.y += velocity.y;
  }
}
void ElectricBall::Draw() {
  if (!is_destroy || time - time_explode_ < 40)
    Projectile::Draw();
}
void ElectricBall::Destroy() {
  if (is_destroy)
    return;
  Projectile::Destroy();
  time_explode_ = time + 1;
}
bool ElectricBall::IsDestroyed() {
  if (is_destroy && time - time_explode_ >= 40)
    return true;
  else
    return false;
}
void ElectricBall::Renew(Vector2 Nposition, bool to_left) {
  Projectile::Renew(Nposition, to_left);
  frame = frame_list[0];
}