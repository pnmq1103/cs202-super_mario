#pragma once
#include <raylib.h>
#include <unordered_map>
#include <string>

enum class BlockType {
      Empty,
      Solid,
      Question,
      Music,
      Ground,
      Rock,
      Pipe
};


enum class PowerUpType {
  None,
  Coin,
  SuperMushroom,
  FireFlower,
  Star,
};

class Block {
protected:
  int Gid;
  Vector2 position;
  Vector2 size;
  BlockType type;
  bool solid;
  bool animating;
  int spriteId;

public:
  Block(
    int gid, Vector2 pos, Vector2 s, BlockType type, bool solid, bool animating,
    int spriteId = 0);
  //virtual functions
  virtual ~Block() = default;
  virtual Block *Clone() const = 0;
  virtual void Update(float dt) = 0;
  virtual void OnHit() = 0;

  //getters and setters
  int GetGid() const;
  Vector2 GetPosition() const;
  Rectangle GetRect() const;
  BlockType GetType() const;
  bool IsSolid() const;
  bool IsAnimating() const;
  int GetSpriteId() const;

  void SetGid(int val);
  void SetPosition(Vector2 pos);
  void SetSpriteId(int id);
  void SetSolid(bool val);
  void SetAnimating(bool val);
  void SetSize(Vector2 s);
  void SetType(int t);

  bool CheckCollision(Rectangle other) const;

  virtual void Render(const Texture &texture) const;
};
