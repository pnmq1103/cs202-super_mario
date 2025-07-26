#pragma once
#include "include/core/application.hpp"
#include <cmath>
#include <fstream>
#include <iostream>
#include <raylib.h>
#include <vector>

class GameObject {
protected:
  Vector2 position;
  const float scale, gravity;
  bool is_destroyed;
  int time;

  const Texture *texture;
  Rectangle frame;
  std::vector<Rectangle> frame_list;

  Vector2 velocity;
  float y_mark;

  Rectangle MakeDestRect(Rectangle rect);
  void LoadFrameList(std::string address);

public:
  GameObject(Vector2 Nposition, float Nscale);
  virtual ~GameObject();
  bool IsDestroyed();
  void Bounce();
  void SetFrameCount();
  virtual void OnHit() = 0;
  virtual void Update();
  virtual void Draw();
  void ReverseDirection();
  virtual void StopY(float Ny);
};