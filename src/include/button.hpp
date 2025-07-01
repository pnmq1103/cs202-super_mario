#pragma once
#include <functional>
#include <raylib.h>
#include <string>

class Button {
private:
  std::function<void()> action_ = {nullptr};
  Rectangle bounds_             = {0.0f, 0.0f, 0.0f, 0.0f};
  std::string label_            = {};
  Texture icon_                 = {};

public:
  ~Button();
  Button() = default;

  void Update();
  void Draw();

  friend class ButtonBuild;
};

class ButtonBuild {
private:
  Button button_;

public:
  ButtonBuild &SetAction(std::function<void()> action);
  ButtonBuild &SetBounds(Rectangle bounds);
  ButtonBuild &SetLabel(const std::string &label);
  ButtonBuild &SetIcon(const std::string &path);
  Button Build();
};
