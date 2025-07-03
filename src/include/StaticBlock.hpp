#pragma once
#include "Block.hpp"

class StaticBlock : public Block {
public:
    StaticBlock(Vector2 pos, int w, int h, BlockType type, int spriteId);

    Block* clone() const override;
};
