/*#include "include/blocks/brick_block.hpp"

BrickBlock::BrickBlock(Vector2 pos, int spriteId)
    : Block(pos, 16, 16, BlockType::Brick, true, spriteId) {}

void BrickBlock::OnHit() {
  isDestroyed = true;
  solid = false;
  spriteId = 999;
}

void BrickBlock::Update(float dt) {

}

bool BrickBlock::IsDestroyed() const {
  return isDestroyed;
}

Block *BrickBlock::Clone() const {
  return new BrickBlock(*this);
}*/
