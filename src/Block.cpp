#include "include/block.hpp"
#include <raylib.h>
#include <unordered_map>

Block::Block(
  Vector2 pos, int w, int h, BlockType type, bool solid, int spriteId)
    : position(pos), width(w), height(h), type(type), solid(solid),
      spriteId(spriteId) {}

void Block::onHit() {}

Vector2 Block::getPosition() const {
  return position;
}

Rectangle Block::getRect() const {
  return {position.x, position.y, (float)width, (float)height};
}

BlockType Block::getType() const {
  return type;
}

bool Block::isSolid() const {
  return solid;
}

int Block::getSpriteId() const {
  return spriteId;
}

void Block::setPosition(Vector2 pos) {
  position = pos;
}

void Block::setSpriteId(int id) {
  spriteId = id;
}

void Block::setSolid(bool val) {
  solid = val;
}

bool Block::checkCollision(Rectangle other) const {
  return CheckCollisionRecs(getRect(), other);
}

void Block::render(
  const Texture2D &texture,
  const std::unordered_map<int, Rectangle> &spriteRects) const {
  auto it = spriteRects.find(spriteId);
  if (it != spriteRects.end()) {
    DrawTextureRec(texture, it->second, position, WHITE);
  }
}
