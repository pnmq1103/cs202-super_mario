#pragma once
#include "include/objects/game_object.hpp"
class QuestionBlock : public GameObject {
private:
  bool is_used_;

public:
  QuestionBlock(Vector2 Nposition, float Nscale);
  ~QuestionBlock();
  void OnHit();
  void Update();
};