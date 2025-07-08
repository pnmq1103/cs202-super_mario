#pragma once
#include <raylib.h>
#include <unordered_map>

enum class BlockType {
  Empty,
  Brick,
  Question,
  Solid,
  Pipe,
  Rock,
  Lava,
};

class Block {
protected:
  Vector2 position;
  int width;
  int height;
  BlockType type;
  bool solid;
  int spriteId;

public:
  Block(
    Vector2 pos, int w, int h, BlockType type, bool solid = true,
    int spriteId = 0);
  virtual ~Block() = default;

  virtual Block *Clone() const = 0;

  virtual void Update(float dt) {}
  virtual void OnHit();

  Vector2 GetPosition() const;
  Rectangle GetRect() const;
  BlockType GetType() const;
  bool IsSolid() const;
  int GetSpriteId() const;

  void SetPosition(Vector2 pos);
  void SetSpriteId(int id);
  void SetSolid(bool val);

  bool CheckCollision(Rectangle other) const;

  virtual void Render(
    const Texture &texture,
    const std::unordered_map<int, Rectangle> &spriteRects) const;
};
