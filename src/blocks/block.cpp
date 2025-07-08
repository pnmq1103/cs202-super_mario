#include "../include/blocks/block.hpp"
#include <raylib.h>
#include <unordered_map>

Block::Block(
  Vector2 pos, int w, int h, BlockType type, bool solid, int spriteId)
    : position(pos), width(w), height(h), type(type), solid(solid),
      spriteId(spriteId) {}

void Block::OnHit() {}

Vector2 Block::GetPosition() const {
  return position;
}

Rectangle Block::GetRect() const {
  return {position.x, position.y, (float)width, (float)height};
}

BlockType Block::GetType() const {
  return type;
}

bool Block::IsSolid() const {
  return solid;
}

int Block::GetSpriteId() const {
  return spriteId;
}

void Block::SetPosition(Vector2 pos) {
  position = pos;
}

void Block::SetSpriteId(int id) {
  spriteId = id;
}

void Block::SetSolid(bool val) {
  solid = val;
}

bool Block::CheckCollision(Rectangle other) const {
  return CheckCollisionRecs(GetRect(), other);
}

void Block::Render(
  const Texture &texture,
  const std::unordered_map<int, Rectangle> &spriteRects) const {
  auto it = spriteRects.find(spriteId);
  if (it != spriteRects.end()) {
    DrawTextureRec(texture, it->second, position, WHITE);
  }
}
