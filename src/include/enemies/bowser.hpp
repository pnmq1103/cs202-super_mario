#pragma once
#include "include/characters/character.hpp"
#include "include/characters/projectile_pool.hpp"
#include "include/enemies/enemy.hpp"

class Bowser : public Enemy {
private:
  int hit_timer_, shoot_timer_, shoot_time_, throw_timer_, throw_duration_;
  Rectangle character_rectangle_;
  Character *character_;
  ProjectilePool *pool_;
  bool stop_direction;

public:
  Bowser(
    Vector2 pos, float Nscale, Character *Ncharacter, ProjectilePool *Npool);
  ~Bowser();
  void OnHit() override;
  EnemyType GetType() override;
  void OnHitFromAbove() override;
  void OnHitFromSide() override;
  void Update() override;
  void Draw() override;
  Enemy *Clone() const override;
  bool IsRunningDeathAnimation() const;
  bool IsCrazy() override;
  void ReverseDirection() override;
  bool GetStopDirection() override;
};