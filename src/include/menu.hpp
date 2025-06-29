#pragma once
#include <raylib.h>
#include <string>
#include <vector>

#include "scene.hpp"

class MenuScene : public Scene {
private:
  Texture background_;
  std::vector<std::string> menu_items_;
  int selected_idx_ = {0};
  double last_input_ = {0.0};
  double cooldown_ = {0.1};

public:
  MenuScene();
  ~MenuScene();

  void Init() override;
  void Update() override;
  void Draw() override;
};
