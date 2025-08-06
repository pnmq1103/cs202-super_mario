#include "include/objects/super_star.hpp"

SuperStar::SuperStar(Vector2 Nposition, float Nscale)
    : GameObject(Nposition, Nscale),
      color_set_({RED, ORANGE, YELLOW, GREEN, BLUE, PURPLE, WHITE}) {
  texture = &App.Resource().GetObject();
  frame   = {25, 153, 15, 16};
}

SuperStar::~SuperStar() {}

void SuperStar::OnHit() {
  if (is_destroyed)
    return;
  is_destroyed = true;
}

void SuperStar::Draw() {
  if (!is_destroyed) {
    DrawTexturePro(
      *texture, frame, MakeDestRect(frame), {0, 0}, 0.f,
      color_set_[(time / 5) % 7]);
  }
}

ObjectType SuperStar::GetType() {
  return ObjectType::SuperStar;
}