#include <raylib.h>
#include <raymath.h>

#include "include/core/application.hpp"
#include "include/core/slider.hpp"

Slider::~Slider() {}

Slider::Slider(
  std ::string label, std::function<void()> action, Rectangle source,
  Rectangle bounds, const std::string &path)
    : Button(label, action, source, bounds, path) {
  slider_bounds_ = {bounds.x + bounds.width + 16, bounds.y + 16, 64 * 5, 32};
}

void Slider::Update() {
  Button::Update();

  if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
    if (CheckCollisionPointRec(GetMousePosition(), slider_bounds_))
      is_dragging_ = true;
  }

  if (is_dragging_) {
    percentage_
      = (GetMousePosition().x - slider_bounds_.x) / slider_bounds_.width;
    percentage_ = Clamp(percentage_, 0, 1);
  }

  if (IsMouseButtonUp(MOUSE_BUTTON_LEFT))
    is_dragging_ = false;
}

void Slider::Draw() {
  Button::Draw();
  DrawRectangleLinesEx(slider_bounds_, 1, BLACK);
  DrawRectangleRec(
    {slider_bounds_.x, slider_bounds_.y, slider_bounds_.width * percentage_,
     slider_bounds_.height},
    BLUE);
}

float Slider::GetPercentage() {
  return percentage_;
}