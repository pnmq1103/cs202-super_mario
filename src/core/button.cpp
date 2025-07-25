#include <functional>
#include <iostream>
#include <raylib.h>
#include <string>
#include <utility>

#include "include/core/button.hpp"

Button::~Button() {
  if (IsTextureValid(icon_))
    UnloadTexture(icon_);
}

Button::Button(
  std::string label, std::function<void()> action, Rectangle source,
  Rectangle bounds, const std::string &path)
    : label_(label), action_(action), dest_(source), bounds_(bounds) {
  icon_ = LoadTexture(path.c_str());
}

void Button::Update() {
  if (Clicked()) {
    if (action_ != nullptr)
      action_();
    else
      throw std::invalid_argument("nullptr");
  }
}

void Button::Draw() {
  DrawTexturePro(icon_, dest_, bounds_, {0, 0}, 0, WHITE);
}

bool Button::Clicked() {
  return IsMouseButtonPressed(MOUSE_BUTTON_LEFT)
         && CheckCollisionPointRec(GetMousePosition(), bounds_);
}