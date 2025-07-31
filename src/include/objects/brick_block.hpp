#pragma once
#include "game_object.hpp"
class BrickBlock : public GameObject {
private:
public:
  BrickBlock(Vector2 Nposition, float Nscale);
  ~BrickBlock();
  void OnHit();
  ObjectType GetType();
};