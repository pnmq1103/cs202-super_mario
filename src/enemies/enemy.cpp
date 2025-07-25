#include "include/enemies/enemy.hpp"
#include <string>

inline EnemyType fromInt(int value) {
  switch (value) {
    case 0:
      return EnemyType::Goomba;
    case 1:
      return EnemyType::Koopa;
    case 2:
      return EnemyType::Piranha;
    case 3:
      return EnemyType::Bowser;
    default:
      return EnemyType::Goomba;
  }
}

Enemy::Enemy(EnemyType type, Vector2 pos, Vector2 size, Vector2 velocity, int spriteId)
    : position(pos), size(size), velocity(velocity), type(type), spriteId(spriteId), alive(true) {}

Rectangle Enemy::GetRect() const {
    return {position.x, position.y, size.x, size.y};
}

Vector2 Enemy::GetPosition() const {
    return position;
}

EnemyType Enemy::GetType() const {
    return type;
}

int Enemy::GetSpriteId() const {
    return spriteId;
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

void Enemy::SetType(int t) {
    type = fromInt(t);
}

bool Enemy::CheckCollision(Rectangle other) const {
    return CheckCollisionRecs(GetRect(), other);
}

void Enemy::ReverseDirection() {
    velocity.x *= -1;
}

void Enemy::Render(Texture &tex, const std::unordered_map<int, Rectangle> &spriteRects) const {
    if (!alive)
        return;
    auto it = spriteRects.find(spriteId);
    if (it != spriteRects.end()) {
        DrawTextureRec(tex, it->second, position, WHITE);
    }
}

