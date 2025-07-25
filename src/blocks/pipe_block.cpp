#include "include/blocks/pipe_block.hpp"
#include "include/items/spawn_item.hpp"

PipeBlock::PipeBlock(
  int gid, Vector2 pos, Vector2 size, int spriteId, bool solid, bool animating)
    : Block(gid, pos, size, BlockType::Rock, spriteId, solid, animating) {}

void PipeBlock::OnHit() {
  return;
}

void PipeBlock::Update(float dt) {}

Block *PipeBlock::Clone() const {
  return new PipeBlock(*this);
}