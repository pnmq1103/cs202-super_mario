#pragma once
#include "include/objects/game_object.hpp"

class Coin : public GameObject {
private:
public:
  Coin(Vector2 Nposition, float Nscale);
  ~Coin();
  void OnHit();
  void Update();
  void Draw();
  ObjectType GetType();
};