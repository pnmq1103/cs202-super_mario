#pragma once
#include "include/blocks/block.hpp"

class StaticBlock : public Block {
public:
  StaticBlock(Vector2 pos, int w, int h, BlockType type, int spriteId);

  Block *Clone() const override;
};
