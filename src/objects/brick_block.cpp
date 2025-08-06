#include "include/objects/brick_block.hpp"
BrickBlock::BrickBlock(Vector2 Nposition, float Nscale)
    : GameObject(Nposition, Nscale) {
  texture = &App.Resource().GetObject();
  frame   = {0, 0, 16, 16};
}

BrickBlock::~BrickBlock() {}

void BrickBlock::OnHit() {
  if (is_destroyed)
    return;
  is_destroyed = true;
  Bounce();
}
ObjectType BrickBlock::GetType() {
  return ObjectType::BrickBlock;
}