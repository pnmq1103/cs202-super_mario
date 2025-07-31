#pragma once
#include "include/objects/game_object.hpp"

class SuperStar : public GameObject {
private:
  const std::vector<Color> color_set_;

public:
  SuperStar(Vector2 Nposition, float Nscale);
  ~SuperStar();
  void OnHit();
  void Draw();
  ObjectType GetType();
};