#pragma once
#include <raylib.h>
#include <unordered_map>

enum class EnemyType {
  Goomba,
  Koopa,
  Piranha,
  Bowser,
};

class Enemy {
protected:
  Vector2 position;
  Vector2 velocity;
  int width, height;
  bool alive;
  int spriteId;
  EnemyType type;

public:
  Enemy(Vector2 pos, int w, int h, EnemyType type, int spriteId);
  virtual ~Enemy() = default;

  virtual void update(float dt) = 0;
  virtual void onHitFromAbove() = 0;
  virtual void onHitFromSide()  = 0;
  virtual Enemy *clone() const  = 0;

  Rectangle getRect() const;
  Vector2 getPosition() const;
  EnemyType getType() const;
  bool isAlive() const;

  void setPosition(Vector2 pos);
  void setAlive(bool status);

  bool checkCollision(Rectangle other) const;

  virtual void
  render(Texture2D &tex, const std::unordered_map<int, Rectangle> &spriteRects)
    const;
};
