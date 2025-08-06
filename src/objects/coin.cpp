#include "include/objects/coin.hpp"

Coin::Coin(Vector2 Nposition, float Nscale) : GameObject(Nposition, Nscale) {
  texture = &App.Resource().GetObject();
  LoadFrameList("res/sprites/objects/objects.txt");
}

Coin::~Coin() {}

void Coin::OnHit() {
  if (is_destroyed)
    return;
  is_destroyed = true;
}

void Coin::Update() {
  frame = frame_list[59 + ((time / 5) % +8)];
  GameObject::Update();
}

void Coin::Draw() {
  if (!is_destroyed)
    GameObject::Draw();
}

ObjectType Coin::GetType() {
  return ObjectType::Coin;
}