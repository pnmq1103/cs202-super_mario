#include "include/objects/fire_flower.hpp"

FireFlower::FireFlower(Vector2 Nposition, float Nscale)
    : GameObject(Nposition, Nscale) {
  texture = &App.Resource().GetObject();
  frame   = {128, 0, 16, 16};
}

FireFlower::~FireFlower() {}

void FireFlower::OnHit() {
  if (is_destroyed)
    return;
  is_destroyed = true;
}

void FireFlower::Draw() {
  if (!is_destroyed)
    GameObject::Draw();
}

ObjectType FireFlower::GetType() {
  return ObjectType::FireFlower;
}