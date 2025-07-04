#pragma once
#include "block.hpp"

class StaticBlock : public Block {
public:
  StaticBlock(Vector2 pos, int w, int h, BlockType type, int spriteId);

  Block *clone() const override;
};
