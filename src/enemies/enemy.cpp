#include "include/enemies/enemy.hpp"

Enemy::Enemy(Vector2 pos, int w, int h, EnemyType type, int spriteId)
    : position(pos), width(w), height(h), type(type), spriteId(spriteId),
      alive(true), velocity({0, 0}) {}

Rectangle Enemy::GetRect() const {
  return {position.x, position.y, (float)width, (float)height};
}

Vector2 Enemy::GetPosition() const {
  return position;
}
EnemyType Enemy::GetType() const {
  return type;
}
bool Enemy::IsAlive() const {
  return alive;
}

void Enemy::SetPosition(Vector2 pos) {
  position = pos;
}
void Enemy::SetAlive(bool status) {
  alive = status;
}

bool Enemy::CheckCollision(Rectangle other) const {
  return CheckCollisionRecs(GetRect(), other);
}

void Enemy::Render(
  Texture &tex, const std::unordered_map<int, Rectangle> &spriteRects) const {
  if (!alive)
    return;
  auto it = spriteRects.find(spriteId);
  if (it != spriteRects.end()) {
    DrawTextureRec(tex, it->second, position, WHITE);
  }
}
