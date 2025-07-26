#include "include/objects/game_object.hpp"

Rectangle GameObject::MakeDestRect(Rectangle rect) {
  return {position.x, position.y, rect.width * scale, rect.height * scale};
}

void GameObject::LoadFrameList(std::string address) {
  std::ifstream fin;
  fin.open(address);
  if (!fin.is_open())
    throw std::runtime_error("cannot open file");
  while (!fin.eof()) {
    Rectangle rect;
    float n;
    fin >> n;
    fin >> n;
    rect.x = n;
    fin >> n;
    rect.y = n;
    fin >> n;
    rect.width = n;
    fin >> n;
    rect.height = n;
    frame_list.push_back(rect);
  }
  fin.close();
}

GameObject::GameObject(Vector2 Nposition, float Nscale)
    : scale(Nscale), gravity(5) {
  position     = Nposition;
  is_destroyed = false;
  time         = 0;
  velocity     = {0, 0};
  y_mark       = position.y;
}

GameObject::~GameObject() {}

void GameObject::SetFrameCount() {
  ++time;
}

void GameObject::Update() {
  velocity.y += gravity;
  position.y += velocity.y;
  if (position.y >= y_mark && !is_destroyed) {
    position.y = y_mark;
    velocity.y = 0;
  }
}

void GameObject::Draw() {
  DrawTexturePro(*texture, frame, MakeDestRect(frame), {0, 0}, 0.f, WHITE);
}

void GameObject::Bounce() {
  velocity.y = -20;
}

bool GameObject::IsDestroyed() {
  return is_destroyed;
}

void GameObject::ReverseDirection() {
  velocity.x *= -1;
}

void GameObject::StopY(float Ny) {}