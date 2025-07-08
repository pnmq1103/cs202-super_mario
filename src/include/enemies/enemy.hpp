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

  virtual void Update(float dt) = 0;
  virtual void OnHitFromAbove() = 0;
  virtual void OnHitFromSide()  = 0;
  virtual Enemy *Clone() const  = 0;

  Rectangle GetRect() const;
  Vector2 GetPosition() const;
  EnemyType GetType() const;
  bool IsAlive() const;

  void SetPosition(Vector2 pos);
  void SetAlive(bool status);

  bool CheckCollision(Rectangle other) const;

  virtual void Render(
    Texture &tex, const std::unordered_map<int, Rectangle> &spriteRects) const;
};
