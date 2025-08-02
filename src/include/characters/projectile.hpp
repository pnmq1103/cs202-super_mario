#pragma once
#include "include/core/application.hpp"
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
  const Texture *texture;
  Rectangle frame;
  std::vector<Rectangle> frame_list;
  Vector2 position, velocity, original_position;
  int time;
  bool is_destroy;
  const float scale;
  void LoadFrameList(std::string address);
  Rectangle MakeDestRect(Rectangle rectangle);

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
  virtual void StopY(bool stop_upper, bool stop_lower) = 0;
  virtual ProjectileType GetType()                     = 0;
  Vector2 GetSpeed();
};