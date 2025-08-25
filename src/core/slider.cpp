#include <raylib.h>
#include <raymath.h>

#include "include/core/application.hpp"
#include "include/core/slider.hpp"

Slider::~Slider() {}

Slider::Slider(
  std ::string label, std::function<void()> action, Rectangle source,
  Rectangle bounds, const std::string &path, float percentage)
    : Button(label, action, source, bounds, path), percentage_(percentage) {
  slider_bounds_ = {bounds.x + bounds.width + 16, bounds.y + 16, 64 * 4, 32};
  target_percentage_ = percentage_;
}

void Slider::Update() {
  Button::Update();

  if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
    if (CheckCollisionPointRec(GetMousePosition(), slider_bounds_))
      is_dragging_ = true;
  }

  if (is_dragging_) {
    target_percentage_
      = (GetMousePosition().x - slider_bounds_.x) / slider_bounds_.width;
    target_percentage_ = Clamp(target_percentage_, 0, 1);
  }

  if (IsMouseButtonUp(MOUSE_BUTTON_LEFT))
    is_dragging_ = false;

  if (Clicked()) {
    if (target_percentage_ == 0)
      target_percentage_ = old_;
    else {
      old_               = target_percentage_;
      target_percentage_ = 0;
    }
  }

  percentage_
    = Lerp(percentage_, target_percentage_, GetFrameTime() * animation_speed_);
  if (fabsf(percentage_ - target_percentage_) < 0.01f) {
    percentage_ = target_percentage_;
  }
}

void Slider::Draw() {
  Button::Draw();
  Color dark_green = {40, 180, 33, 255};
  DrawRectangleLinesEx(slider_bounds_, 1, dark_green);
  DrawRectangleRec(
    {slider_bounds_.x, slider_bounds_.y, slider_bounds_.width * percentage_,
     slider_bounds_.height},
    dark_green);
}

float Slider::GetPercentage() {
  return percentage_;
}