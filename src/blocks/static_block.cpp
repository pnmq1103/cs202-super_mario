#include "../include/blocks/static_block.hpp"

StaticBlock::StaticBlock(
  Vector2 pos, int w, int h, BlockType type, int spriteId)
    : Block(pos, w, h, type, true, spriteId) {}

Block *StaticBlock::clone() const {
  return new StaticBlock(position, width, height, type, spriteId);
}
