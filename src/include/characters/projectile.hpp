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
  Vector2 position, velocity, original_position;
  int time;
  bool is_destroy;
  const float scale;
  void LoadFrameList(std::string address);

public:
  Projectile(float Nscale, Vector2 Nposition, Vector2 Nvelocity);
  virtual ~Projectile();
  void SetFrameCount();
  virtual void Draw();
  Rectangle GetRectangle();
  virtual void Destroy();
  virtual bool IsDestroyed();
  void Renew(Vector2 Nposition, bool to_left);
};