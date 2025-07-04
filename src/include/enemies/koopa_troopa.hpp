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
  void update(float dt) override;
  void onHitFromAbove() override;
  void onHitFromSide() override;
  Enemy *clone() const override;

  KoopaState getState() const;
};
