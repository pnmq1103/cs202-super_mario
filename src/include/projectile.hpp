#pragma once
#include <fstream>
#include <iostream>
#include <raylib.h>
#include <vector>
class Projectile {
protected:
  Texture texture;
  Rectangle frame;
  std::vector<Rectangle> frame_list;
  Vector2 pos, v, ori_pos;
  int bounce_count, t;
  bool destroy;
  const float scale;
  void LoadFrameList(std::string at);

public:
  Projectile(float Nscale, Vector2 Npos, Vector2 Nv);
  virtual ~Projectile();
  void SetFrameCount();
  virtual void Draw();
  Rectangle GetRectangle();
  virtual void Destroy();
};