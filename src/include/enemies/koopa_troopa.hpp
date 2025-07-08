#pragma once
#include "enemy.hpp"

enum class KoopaState {
  Walking,
  Shell,
  Sliding,
};

class KoopaTroopa : public Enemy {
private:
  KoopaState state;
  float shellTimer = 0;

public:
  KoopaTroopa(Vector2 pos, int spriteId);
  void Update(float dt) override;
  void OnHitFromAbove() override;
  void OnHitFromSide() override;
  Enemy *Clone() const override;

  KoopaState GetState() const;
};
