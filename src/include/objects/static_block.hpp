#pragma once
#include "include/objects/game_object.hpp"

class StaticBlock : public GameObject {
private:
public:
  StaticBlock(Vector2 Nposition, float Nscale, char theme);
  // g for ground, s for sky, u for underground , w for underwater
  ~StaticBlock();
  void OnHit();
  ObjectType GetType();
};