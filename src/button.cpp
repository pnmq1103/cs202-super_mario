#include "include/button.hpp"

Button::~Button() {
  UnloadTexture(icon_);
}

void Button::Update() {
  DrawTexture(icon_, 50, 50, RAYWHITE);
}

void Button::Draw() {
  DrawRectangleRec(bounds_, BLACK);
}

ButtonBuild &ButtonBuild::SetAction(std::function<void()> action) {
  button_.action_ = std::move(action);
  return *this;
}

ButtonBuild &ButtonBuild::SetBounds(Rectangle bounds) {
  button_.bounds_ = bounds;
  return *this;
}

ButtonBuild &ButtonBuild::SetLabel(const std::string &label) {
  button_.label_ = label;
  return *this;
}

ButtonBuild &ButtonBuild::SetIcon(const std::string &path) {
  button_.icon_ = LoadTexture(path.c_str());
  return *this;
}

Button ButtonBuild::Build() {
  return std::move(button_);
}