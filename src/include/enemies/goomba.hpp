#pragma once
#include "enemy.hpp"

class Goomba : public Enemy {
public:
  Goomba(Vector2 pos, int spriteId);
  void update(float dt) override;
  void onHitFromAbove() override;
  void onHitFromSide() override;
  Enemy *clone() const override;
};
