#include "include/blocks/solid_block.hpp"
#include "include/items/spawn_item.hpp"

SolidBlock::SolidBlock(
  int gid, Vector2 pos, Vector2 size, int spriteId, bool solid, bool animating)
    : Block(gid, pos, size, BlockType::Solid, spriteId, solid, animating) {}

void SolidBlock::OnHit() {
  return;
}

void SolidBlock::Update(float dt) {}

Block *SolidBlock::Clone() const {
  return new SolidBlock(*this);
}
