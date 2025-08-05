#pragma once
#include "include/enemies/enemy.hpp"

class Bowser : public Enemy {
private:
  int hp;
  float fire_timer;
  float attack_cooldown;
  bool rage_mode;

public:
  Bowser(Vector2 pos, float Nscale);
  ~Bowser();

  void OnHit() override;
  void Update() override;
  void OnHitFromAbove() override;
  void OnHitFromSide() override;
  EnemyType GetType() override;
  Enemy *Clone() const override;

  // Boss-specific methods (simplified)
  int GetHP() const;
  bool IsInRageMode() const;
  void EnterRageMode(); // Simplified rage mode (not used)
  void ShootFireball(); // Simplified (no actual shooting)
};