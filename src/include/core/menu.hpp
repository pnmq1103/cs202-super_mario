#pragma once
#include <raylib.h>
#include <string>
#include <unordered_map>
#include <vector>

#include "include/core/button.hpp"
#include "include/core/scene.hpp"

class MenuScene : public Scene {
private:
  SceneType type_ = {Menu};

  Texture background_ = {};
  std::vector<std::string> menu_items_;
  int selected_idx_ = {0};

  double last_input_     = {0};
  const double cooldown_ = {0.2};

  float timer_    = {0};
  float duration_ = {0.5f};
  bool reset_     = {false};

  std::vector<Button> buttons_;

public:
  MenuScene();
  ~MenuScene();

  void Init() override;
  void Update() override;
  void Draw() override;
  SceneType Type() override;
  void Resume() override;

  void DrawOptions();

  void CreateButtons();
  void UpdateButtons();
  void DrawButtons();
};
