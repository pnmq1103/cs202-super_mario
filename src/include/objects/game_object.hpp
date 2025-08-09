#pragma once
#include "include/core/application.hpp"
#include <cmath>
#include <fstream>
#include <iostream>
#include <raylib.h>
#include <vector>

enum class ObjectType {
  SuperMushroom,
  Coin,
  FireFlower,
  SuperStar,
  Block,
  BrickBlock,
};

class GameObject {
private:
  static int object_count_;
  int index_;

protected:
  static int time;

  Vector2 position;
  const float scale, gravity;
  bool is_destroyed;

  const Texture *texture;
  Rectangle frame;
  std::vector<Rectangle> frame_list;

  Vector2 velocity;
  float y_mark;

  Rectangle MakeDestRect(Rectangle rect);
  void LoadFrameList(std::string address);
  void Bounce();

public:
  static void SetFrameCount();
  static void Reset();

  GameObject(Vector2 Nposition, float Nscale);
  virtual ~GameObject();
  virtual Rectangle GetRectangle();
  bool IsDestroyed();
  virtual void OnHit() = 0;
  virtual void Update();
  virtual void Draw();
  void ReverseDirection();
  virtual void StopY(float Ny);
  virtual ObjectType GetType() = 0;
  virtual bool IsPipe();
  Vector2 GetSpeed();
};
