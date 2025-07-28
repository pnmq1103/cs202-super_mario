#include "include/blocks/ground_block.hpp"
#include "include/items/spawn_item.hpp"

GroundBlock::GroundBlock(
  int gid, Vector2 pos, Vector2 size, int spriteId, bool solid, bool animating)
    : Block(gid, pos, size, BlockType::Ground, spriteId, solid, animating) {}

void GroundBlock::OnHit() {
  return;
}

void GroundBlock::Update(float dt) {}

Block *GroundBlock::Clone() const {
  return new GroundBlock(*this);
}
