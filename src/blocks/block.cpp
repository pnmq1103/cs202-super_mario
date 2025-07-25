#include "include/blocks/block.hpp"
#include <raylib.h>
#include <unordered_map>

inline BlockType fromInt(int type) {
    switch (type) {
      case 0:
        return BlockType::Empty;
      case 1:
        return BlockType::Solid;
      case 2:
        return BlockType::Question;
      case 3:
        return BlockType::Music;
      case 4:
        return BlockType::Ground;
      case 5:
        return BlockType::Rock;
      case 6:
        return BlockType::Pipe;
      default:
        return BlockType::Empty;
    }
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

void Block::SetType(int t) {
  type = fromInt(t);
}
    bool Block::CheckCollision(Rectangle other) const {
  return CheckCollisionRecs(GetRect(), other);
}

    //game manager will get block gid via getGid() then extract the texture from resource manager
void Block::Render(const Texture& texture) const {
  DrawTexture(texture, position.x, position.y, RAYWHITE);
}
