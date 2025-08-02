#pragma once
#include "include/characters/projectile.hpp"

class MarioFireball : public Projectile {
private:
  float gravity_, bounce_coefficient_, range_;

public:
  MarioFireball();
  ~MarioFireball();
  void Update();
  void Draw();
  void Renew(Vector2 Nposition, bool to_left);
  void StopY(bool stop_upper, bool stop_lower);
  ProjectileType GetType();
};

class EnemyFireball : public Projectile {
private:
  float range_;

public:
  EnemyFireball();
  ~EnemyFireball();
  void Update();
  void Draw();
  void StopY(bool stop_upper, bool stop_lower);
  ProjectileType GetType();
};

class ElectricBall : public Projectile {
private:
  int time_explode_;
  bool is_explode_;
  float range_;

public:
  ElectricBall();
  ~ElectricBall();
  void Update();
  void Draw();
  void Destroy();
  void Renew(Vector2 Nposition, bool to_left);
  void StopY(bool stop_upper, bool stop_lower);
  ProjectileType GetType();
};