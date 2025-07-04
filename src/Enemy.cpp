#include "Enemy.hpp"

Enemy::Enemy(Vector2 pos, int w, int h, EnemyType type, int spriteId)
    : position(pos), width(w), height(h), type(type), spriteId(spriteId), alive(true), velocity({0, 0}) {}

Rectangle Enemy::getRect() const {
    return { position.x, position.y, (float)width, (float)height };
}

Vector2 Enemy::getPosition() const { return position; }
EnemyType Enemy::getType() const { return type; }
bool Enemy::isAlive() const { return alive; }

void Enemy::setPosition(Vector2 pos) { position = pos; }
void Enemy::setAlive(bool status) { alive = status; }

bool Enemy::checkCollision(Rectangle other) const {
    return CheckCollisionRecs(getRect(), other);
}

void Enemy::render(Texture2D& tex, const std::unordered_map<int, Rectangle>& spriteRects) const {
    if (!alive) return;
    auto it = spriteRects.find(spriteId);
    if (it != spriteRects.end()) {
        DrawTextureRec(tex, it->second, position, WHITE);
    }
}
