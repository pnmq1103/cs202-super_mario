#pragma once
#include "include/objects/game_object.hpp"

class SuperMushroom : public GameObject {
private:
  bool is_used_ = false;

public:
  SuperMushroom(Vector2 Nposition, float Nscale, bool to_left);
  ~SuperMushroom();
  void OnHit();
  void Update();
  void Draw();
  void StopY(float Ny);
  ObjectType GetType();
};