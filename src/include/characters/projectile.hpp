#pragma once
#include <fstream>
#include <iostream>
#include <raylib.h>
#include <vector>

enum ProjectileType {
  mario_fireball,
  enemy_fireball,
  electric_ball,
};

class Projectile {
protected:
  Texture texture;
  Rectangle frame;
  std::vector<Rectangle> frame_list;
  Vector2 position, velocity, original_position;
  int time;
  bool is_destroy;
  const float scale;
  void LoadFrameList(std::string address);

public:
  Projectile(float Nscale, Vector2 Nvelocity);
  virtual ~Projectile();
  void SetFrameCount();
  virtual void Update() = 0;
  virtual void Draw();
  Rectangle GetRectangle();
  virtual void Destroy();
  bool IsDestroyed();
  virtual void Renew(Vector2 Nposition, bool to_left);
  virtual void StopY(float upper_bounce, float lower_bounce) = 0;
  virtual ProjectileType GetType()                           = 0;
};