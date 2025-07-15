#pragma once
#include "include/blocks/block.hpp"

class BrickBlock : public Block {
private:
  bool isDestroyed = false;

public:
  BrickBlock(Vector2 pos, int spriteId = 0);

  void OnHit() override;
  void Update(float dt) override;

  bool IsDestroyed() const;

  Block *Clone() const override;
};
