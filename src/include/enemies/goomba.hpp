#pragma once
#include "include/enemies/enemy.hpp"

class Goomba : public Enemy {
public:
  Goomba(Vector2 pos, int spriteId);
  void Update(float dt) override;
  void OnHitFromAbove() override;
  void OnHitFromSide() override;
  Enemy *Clone() const override;
};
