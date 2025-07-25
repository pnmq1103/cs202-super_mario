#pragma once
#include <raylib.h>
#include <unordered_map>
#include <string>

enum class EnemyType {
  Goomba,
  Koopa,
  Piranha,
  Bowser,
};

class Enemy {
protected:
  Vector2 position;
  Vector2 size;
  Vector2 velocity;
  bool alive;
  int spriteId;
  EnemyType type;

public:
  Enemy(EnemyType type, Vector2 pos, Vector2 size, Vector2 velo, int spriteId);
  virtual ~Enemy() = default;

  virtual void Update(float dt) = 0;
  virtual void OnHitFromAbove() = 0;
  virtual void OnHitFromSide()  = 0;
  virtual Enemy *Clone() const  = 0;

  Rectangle GetRect() const;
  Vector2 GetPosition() const;
  EnemyType GetType() const;
  int GetSpriteId() const;
  bool IsAlive() const;

  void SetPosition(Vector2 pos);
  void SetAlive(bool status);
  void SetType(int t);
  bool CheckCollision(Rectangle other) const;
  void ReverseDirection();

  virtual void Render(
    Texture &tex, const std::unordered_map<int, Rectangle> &spriteRects) const;
};
