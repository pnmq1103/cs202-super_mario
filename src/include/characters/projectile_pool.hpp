#pragma once
#include <vector>

#include "include/characters/fireball.hpp"
#include "include/collision/collision_handler.hpp"

class ProjectilePool {
private:
  std::vector<Projectile *> projectile_list_;

public:
  ProjectilePool();
  ~ProjectilePool();
  void ShootMarioFireball(Vector2 Nposition, bool to_left);
  void ShootEnemyFireball(Vector2 Nposition, bool to_left);
  void ShootElectricBall(Vector2 Nposition, bool to_left);
  void SetFrameCount();
  void Update();
  void Draw();
};