#include "include/projectile.hpp"
class MarioFireball : public Projectile {
private:
  float gravity_, bounce_co_, range_;

public:
  MarioFireball(Vector2 Npos, bool left);
  ~MarioFireball();
  void Update(float UpBounce, float LowBounce);
  void Draw();
};

class EnemyFireball : public Projectile {
public:
  EnemyFireball(Vector2 Npos, bool left);
  ~EnemyFireball();
  void Update();
  void Draw();
};

class ElectricBall : public Projectile {
private:
  int t_explose_;

public:
  ElectricBall(Vector2 Npos, bool left);
  ~ElectricBall();
  void Update();
  void Draw();
  void Destroy();
};