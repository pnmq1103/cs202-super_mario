#pragma once
#include "include/enemies/enemy.hpp"

class Bowser : public Enemy {
private:
  int hp;
  float fire_timer;
  float attack_cooldown;
  bool rage_mode; // New member for rage mode

public:
  Bowser(Vector2 pos, float Nscale);
  ~Bowser();
  
  void OnHit() override;
  void Update() override;
  void OnHitFromAbove() override;
  void OnHitFromSide() override;
  EnemyType GetType() override;
  Enemy *Clone() const override;

  // Boss-specific methods
  int GetHP() const; // Fixed method name
  bool IsInRageMode() const; // New method
  void EnterRageMode(); // Special mode when low health
  void PerformSpecialAttack(); // Boss special attack
  void ShootFireball(); // Shoot fireball at player
  Vector2 GetCenterPosition() const; // Get center for projectile targeting
};
