#include "include/blocks/rock_block.hpp"
#include "include/items/spawn_item.hpp"

RockBlock::RockBlock(
  int gid, Vector2 pos, Vector2 size, int spriteId, bool solid, bool animating)
    : Block(gid, pos, size, BlockType::Rock, spriteId, solid, animating) {
}


void RockBlock::OnHit() {
  return;
}

void RockBlock::Update(float dt) {}

Block *RockBlock::Clone() const {
  return new RockBlock(*this);
}
