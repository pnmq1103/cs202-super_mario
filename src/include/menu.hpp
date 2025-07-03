#pragma once
#include <raylib.h>
#include <string>
#include <vector>

#include "button.hpp"
#include "scene.hpp"
#include <unordered_map>

class MenuScene : public Scene {
private:
  Texture background_ = {};
  std::vector<std::string> menu_items_;
  int selected_idx_ = {0};

  double last_input_     = {0.0};
  const double cooldown_ = {0.2};

  Texture buttons_texture = {};
  std::vector<Button> buttons_;
  std::unordered_map<int, Rectangle> buttons_info_;

public:
  enum MenuOption { Game, Load, Editor, Exit };

public:
  MenuScene();
  ~MenuScene();

  void Init() override;
  void Update() override;
  void Draw() override;

  void CreateButtons();
};
