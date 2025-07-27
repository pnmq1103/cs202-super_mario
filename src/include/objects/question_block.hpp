#pragma once
#include "include/objects/game_object.hpp"

enum QuestionBlockItem {
  coin,
  fire_flower,
  super_mushroom,
  super_star,
};

class QuestionBlock : public GameObject {
private:
  bool is_used_, item_spawned_;
  QuestionBlockItem item_;

public:
  QuestionBlock(Vector2 Nposition, float Nscale, QuestionBlockItem Nitem);
  ~QuestionBlock();
  void OnHit();
  void Update();
};