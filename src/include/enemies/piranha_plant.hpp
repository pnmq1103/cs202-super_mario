#pragma once
#include "include/enemies/enemy.hpp"

class PiranhaPlant : public Enemy {
private:
  float time     = 0;
  float interval = 2.0f;
  bool up        = false;

public:
  PiranhaPlant(Vector2 pos, Vector2 size, Vector2 velo, int spriteId);
  void Update(float dt) override;
  void OnHitFromAbove() override;
  void OnHitFromSide() override;
  Enemy *Clone() const override;
};
