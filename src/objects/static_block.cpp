#include "include/objects/static_block.hpp"
StaticBlock::StaticBlock(Vector2 Nposition, float Nscale, char theme)
    : GameObject(Nposition, Nscale) {
  switch (theme) {
    case 'g':
      texture = &App.Resource().GetTileset('m');
      frame   = {80, 64, 16, 16};
      break;
    case 's':
      texture = &App.Resource().GetTileset('s');
      frame   = {205, 119, 16, 16};
      break;
    case 'u':
      texture = &App.Resource().GetTileset('u');
      frame   = {52, 204, 16, 16};
      break;
    case 'w':
      texture = &App.Resource().GetTileset('w');
      frame   = {69, 204, 16, 16};
      break;
    default:
      throw "invalid theme";
      break;
  }
}

StaticBlock::StaticBlock(Vector2 Nposition, float Nscale, Rectangle rect)
    : GameObject(Nposition, Nscale) {
  texture = &App.Resource().GetTileset('m');
  frame = rect;
}

StaticBlock::~StaticBlock() {}

void StaticBlock::OnHit() {}

ObjectType StaticBlock::GetType() {
  return ObjectType::Block;
}