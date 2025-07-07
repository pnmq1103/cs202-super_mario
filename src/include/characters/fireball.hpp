#include "include/characters/projectile.hpp"
class MarioFireball : public Projectile {
private:
  float gravity_, bounce_coefficient_, range_;

public:
  MarioFireball(Vector2 Nposition, bool to_left);
  ~MarioFireball();
  void Update(float upper_bounce, float lower_bounce);
  void Draw();
};

class EnemyFireball : public Projectile {
public:
  EnemyFireball(Vector2 Nposition, bool to_left);
  ~EnemyFireball();
  void Update();
  void Draw();
};

class ElectricBall : public Projectile {
private:
  int time_explode_;

public:
  ElectricBall(Vector2 Nposition, bool to_left);
  ~ElectricBall();
  void Update();
  void Draw();
  void Destroy();
  bool IsDestroyed();
  void Renew(Vector2 Nposition, bool to_left);
};