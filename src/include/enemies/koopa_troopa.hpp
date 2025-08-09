#pragma once
#include "include/enemies/enemy.hpp"

enum class KoopaState {
  Walking,
  Shell,
  Sliding,
};

class KoopaTroopa : public Enemy {
private:
  KoopaState koopa_state;
  float shell_timer;

public:
  KoopaTroopa(Vector2 pos, float Nscale);
  ~KoopaTroopa();

  void OnHit() override;
  void Update() override;
  void OnHitFromAbove() override;
  void OnHitFromSide() override;
  EnemyType GetType() override;
  Enemy *Clone() const override;

  KoopaState GetKoopaState() const;
  void KickShell(bool kick_left); // New method for manual shell kicking

  bool IsRunningDeathAnimation() const;
};
