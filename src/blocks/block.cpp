#include "include/blocks/block.hpp"
#include <raylib.h>
#include <unordered_map>

inline BlockType fromString(const std::string &s) {
  if (s == "Empty")
    return BlockType::Empty;
  if (s == "Brick")
    return BlockType::Brick;
  if (s == "Question")
    return BlockType::Question;
  if (s == "Solid")
    return BlockType::Solid;
  if (s == "Pipe")
    return BlockType::Pipe;
  if (s == "Rock")
    return BlockType::Rock;
  if (s == "Lava")
    return BlockType::Lava;
}

Block::Block(
  Vector2 pos, int w, int h, BlockType type, bool solid, int spriteId)
    : position(pos), width(w), height(h), type(type), solid(solid),
      spriteId(spriteId) {}

void Block::Update(float dt) {}

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

void Block::SetSize(int w, int h) {
  width  = w;
  height = h;
}

void Block::SetType(std::string t) {
  type = fromString(t);
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
