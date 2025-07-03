#include "include/fireball.hpp"

MarioFireball::MarioFireball(Vector2 Npos, bool left)
    : Projectile(5, Npos, {10, 0}) {
  gravity_   = 5;
  bounce_co_ = 1;
  if (left)
    v.x *= -1;
  range_ = 100000;

  LoadFrameList("res/sprites/icons/objects.txt");
  Image image = LoadImage("res/sprites/icons/objects.png");
  ImageResize(&image, image.width * scale, image.height * scale);
  texture = LoadTextureFromImage(image);
  UnloadImage(image);
  frame = frame_list[133];
}
MarioFireball::~MarioFireball() {}
void MarioFireball::Update(float UpBounce, float LowBounce) {
  frame = frame_list[(t / 15) % 3 + 133];
  if (v.x > 0) {
    if (pos.x >= ori_pos.x + range_)
      destroy = true;
  } else {
    if (pos.x <= ori_pos.x - range_)
      destroy = true;
  }
  if (pos.y <= UpBounce)
    v.y = 0;
  else if (pos.y + frame.height >= LowBounce)
    v.y *= -bounce_co_;
  else
    v.y += gravity_;
  pos.x += v.x;
  pos.y += v.y;
}
void MarioFireball::Draw() {
  if (!destroy)
    Projectile::Draw();
}

EnemyFireball::EnemyFireball(Vector2 Npos, bool left)
    : Projectile(5, Npos, {10, 0}) {
  if (left)
    v.x *= -1;

  LoadFrameList("res/sprites/icons/objects.txt");
  Image image = LoadImage("res/sprites/icons/objects.png");
  ImageResize(&image, image.width * scale, image.height * scale);
  texture = LoadTextureFromImage(image);
  UnloadImage(image);
  frame = frame_list[133];
}
EnemyFireball::~EnemyFireball() {}
void EnemyFireball::Update() {
  if ((t / 5) % 3 == 0) {
    frame = frame_list[745];
  } else if ((t / 5) % 3 == 1) {
    frame = frame_list[759];
  } else {
    frame = frame_list[773];
  }
  pos.x += v.x;
  pos.y += v.y;
}
void EnemyFireball::Draw() {
  if (!destroy)
    Projectile::Draw();
}

ElectricBall::ElectricBall(Vector2 Npos, bool left)
    : Projectile(1, Npos, {10, 0}) {
  if (left)
    v.x *= -1;

  LoadFrameList("res/sprites/electric_shot/electric_shot.txt");
  Image image = LoadImage("res/sprites/electric_shot/electric_shot.png");
  ImageResize(&image, image.width * scale, image.height * scale);
  texture = LoadTextureFromImage(image);
  UnloadImage(image);
  frame = frame_list[0];
}
ElectricBall::~ElectricBall() {}
void ElectricBall::Update() {
  if (destroy) {
    frame = frame_list[(t - t_explose_) / 5];
  } else {
    pos.x += v.x;
    pos.y += v.y;
  }
}
void ElectricBall::Draw() {
  if (!destroy || t - t_explose_ < 40)
    Projectile::Draw();
}
void ElectricBall::Destroy() {
  if (destroy)
    return;
  Projectile::Destroy();
  t_explose_ = t + 1;
}