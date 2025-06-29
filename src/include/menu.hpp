#pragma once
#include <raylib.h>
#include <string>
#include <vector>

#include "scene.hpp"

class MenuScene : public Scene {
private:
  Texture background_ = {};
  std::vector<std::string> menu_items_;
  int selected_idx_ = {0};

  double last_input_     = {0.0};
  const double cooldown_ = {0.2};

public:
  enum MenuOption { Game, Character, Editor, Setting };

public:
  MenuScene();
  ~MenuScene();

  void Init() override;
  void Update() override;
  void Draw() override;
};
