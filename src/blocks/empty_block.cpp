#include "include/blocks/empty_block.hpp"
#include "include/items/spawn_item.hpp"

EmptyBlock::EmptyBlock(
  int gid, Vector2 pos, Vector2 size, int spriteId, bool solid, bool animating)
    : Block(gid, pos, size, BlockType::Empty, spriteId, solid, animating) {}

void EmptyBlock::OnHit() {
  return;
}

void EmptyBlock::Update(float dt) {}

Block *EmptyBlock::Clone() const {
  return new EmptyBlock(*this);
}
