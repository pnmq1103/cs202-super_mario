#pragma once
#include "include/enemies/enemy.hpp"

class Bowser : public Enemy {
private:
  int hp;
  float fireTimer = 0;

public:
  Bowser(Vector2 pos, Vector2 size, Vector2 velo, int spriteId);
  void Update(float dt) override;
  void OnHitFromAbove() override;
  void OnHitFromSide() override;
  Enemy *Clone() const override;

  int getHP() const;
};
