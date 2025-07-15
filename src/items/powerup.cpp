#include "include/items/powerup.hpp"

PowerUp::PowerUp(Vector2 pos, int spriteId)
  : position(pos), spriteId(spriteId) {}

void PowerUp::Update(float dt) {
  position.y += 30 * dt;
}

void PowerUp::Render(const Texture& texture) {
  DrawTextureRec(texture, {(float)(spriteId * 16), 0, 16, 16}, position, WHITE);
}

Rectangle PowerUp::GetRect() const {
  return {position.x, position.y, 16, 16};
}

bool PowerUp::IsActive() const {
  return active;
}

void PowerUp::Deactivate() {
  active = false;
}
