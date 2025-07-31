#pragma once
#include "include/objects/game_object.hpp"

class FireFlower : public GameObject {
private:
public:
  FireFlower(Vector2 Nposition, float Nscale);
  ~FireFlower();
  void OnHit();
  void Draw();
  ObjectType GetType();
};