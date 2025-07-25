#pragma once
#include "include/blocks/question_block.hpp"
#include <raylib.h>

class PowerUp {
protected:
  Vector2 position;
  bool active = true;
  int spriteId;

public:
  PowerUp(Vector2 pos, int spriteId = 0);
  virtual ~PowerUp() = default;

  virtual void Update(float dt);
  virtual void Render(const Texture &texture);
  virtual void ApplyEffect()          = 0;
  virtual PowerUpType GetType() const = 0;

  Rectangle GetRect() const;
  bool IsActive() const;
  void Deactivate();
};
