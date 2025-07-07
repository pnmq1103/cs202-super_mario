#include <functional>
#include <iostream>
#include <raylib.h>
#include <string>
#include <utility>

#include "../include/core/button.hpp"

Button::~Button() {
  if (IsTextureValid(icon_))
    UnloadTexture(icon_);
}

Button::Button(
  std::string label, std::function<void()> action, Rectangle source,
  Rectangle bounds, const std::string &path)
    : label_(label), action_(action), source_(source), bounds_(bounds) {
  icon_ = LoadTexture(path.c_str());
}

void Button::Update() {
  if (
    IsMouseButtonPressed(MOUSE_BUTTON_LEFT)
    && CheckCollisionPointRec(GetMousePosition(), bounds_)) {
    if (action_ != nullptr)
      action_();
    else
      throw std::invalid_argument("nullptr");
  }
}

void Button::Draw() {
  DrawTexturePro(icon_, source_, bounds_, {0, 0}, 0, WHITE);
}