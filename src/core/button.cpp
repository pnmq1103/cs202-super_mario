#include <cmath>
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
  std::string label, std::function<void()> action, Rectangle src, Rectangle dst,
  const std::string &path)
    : label_(label), action_(action), src_(src), dst_(dst) {
  icon_ = LoadTexture(path.c_str());
}

void Button::Update() {
  if (Clicked())
    Activate();

  is_hovered_ = Hovered();
}

void Button::Draw() {
  if (is_highlighted_ || is_hovered_) {
    tint_ = WHITE;

    Font font         = GetFontDefault();
    const char *text  = label_.c_str();
    float font_size   = 20;
    float spacing     = 1;
    Vector2 text_size = MeasureTextEx(font, text, font_size, spacing);
    Vector2 pos       = {
      dst_.x + (dst_.width - text_size.x) / 2,
      dst_.y + dst_.height + text_size.y * 0.75f};

    // Rectangle
    Vector2 padding = {10, 3};
    Rectangle rec   = {
      pos.x - padding.x, pos.y - padding.y, text_size.x + padding.x * 2,
      text_size.y + padding.y * 2};
    float rounded = 0.7f;
    int seg       = 20;
    DrawRectangleRounded(rec, rounded, seg, BLACK);
    DrawRectangleRoundedLinesEx(rec, rounded, seg, 1, GRAY);

    // Triangle
    float length   = padding.y * 2;
    float center_x = rec.x + rec.width / 2;
    Vector2 a      = {center_x, rec.y - length};
    Vector2 b      = {center_x - length * cosf(PI / 6), rec.y};
    Vector2 c      = {center_x + length * cosf(PI / 6), rec.y};
    DrawTriangle(a, b, c, BLACK);

    // Draw text
    DrawTextEx(font, text, pos, font_size, spacing, WHITE);
  } else
    tint_ = Fade(LIGHTGRAY, 0.9f);

  DrawTexturePro(icon_, src_, dst_, {0, 0}, 0, tint_);
}

void Button::Activate() {
  if (action_ != nullptr)
    action_();
  else
    throw std::invalid_argument("nullptr");
}

void Button::ToggleHighlight() {
  is_highlighted_ = !is_highlighted_;
}

bool Button::Clicked() {
  return IsMouseButtonPressed(MOUSE_BUTTON_LEFT)
         && CheckCollisionPointRec(GetMousePosition(), dst_);
}

bool Button::Hovered() {
  return CheckCollisionPointRec(GetMousePosition(), dst_);
}