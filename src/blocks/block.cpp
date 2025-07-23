#include "include/blocks/block.hpp"
#include <raylib.h>
#include <unordered_map>

inline BlockType fromString(const std::string &s) {
  if (s == "EMPTY_BLOCK")
    return BlockType::Empty;
  if (s == "SOLID_BLOCK")
    return BlockType::Solid;
  if (s == "QUESTION_BLOCK")
    return BlockType::Question;
  if (s == "MUSIC_BLOCK")
    return BlockType::Music;
  if (s == "GROUND_BLOCK")
    return BlockType::Ground;
  if (s == "ROCK_BLOCK")
    return BlockType::Rock;
}

Block::Block(
  int gid, Vector2 pos, Vector2 s, BlockType type, bool solid, bool animating, int spriteId)
    : Gid(gid), position(pos), size(s), type(type), solid(solid), animating(animating), spriteId(spriteId) {}

int Block::GetGid() const {
  return Gid;
}

Vector2 Block::GetPosition() const {
  return position;
}

Rectangle Block::GetRect() const {
  return {position.x, position.y, size.x, size.y};
}

BlockType Block::GetType() const {
  return type;
}

bool Block::IsSolid() const {
  return solid;
}

bool Block::IsAnimating() const {
  return animating;
}

int Block::GetSpriteId() const {
  return spriteId;
}

void Block::SetGid(int val) {
  Gid = val;
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

void Block::SetAnimating(bool val) {
  animating = val;
}

void Block::SetSize(Vector2 s) {
  size = s;
}

void Block::SetType(std::string t) {
  type = fromString(t);
}
    bool Block::CheckCollision(Rectangle other) const {
  return CheckCollisionRecs(GetRect(), other);
}

    //game manager will get block gid via getGid() then extract the texture from resource manager
void Block::Render(const Texture& texture) const {
  DrawTexture(texture, position.x, position.y, RAYWHITE);
}
