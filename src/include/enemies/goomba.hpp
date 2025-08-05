#pragma once
#include "include/enemies/enemy.hpp"

class Goomba : public Enemy {
public:
  Goomba(Vector2 pos, float Nscale);
  ~Goomba();

  void OnHit() override;
  void Update() override;
  void OnHitFromAbove() override;
  void OnHitFromSide() override;
  EnemyType GetType() override;
  Enemy *Clone() const override;
};
