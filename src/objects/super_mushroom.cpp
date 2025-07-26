#include "include/objects/super_mushroom.hpp"

SuperMushroom::SuperMushroom(Vector2 Nposition, float Nscale, bool to_left)
    : GameObject(Nposition, Nscale) {
  velocity.x = 5;
  if (to_left)
    velocity.x *= -1;
  texture = &App.Resource().GetIcon();
  frame   = {46, 131, 16, 16};
  y_mark  = INT_MAX / 2;
}

SuperMushroom::~SuperMushroom() {}

void SuperMushroom::OnHit() {
  if (is_destroyed)
    return;
  is_destroyed = true;
}

void SuperMushroom::Draw() {
  if (!is_destroyed)
    GameObject::Draw();
}

void SuperMushroom::StopY(float Ny) {
  velocity.y = -gravity;
  position.y = Ny - frame.height;
}