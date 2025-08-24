#pragma once
#include <vector>

#include "include/core/button.hpp"
#include "include/core/constants.hpp"
#include "include/core/scene.hpp"

class PauseScene : public Scene {
private:
  SceneType type_ = {Pause};

  std::vector<Button> buttons_;

  float box_width  = 600;
  float box_height = 200;
  Vector2 box_pos  = {
    (constants::screenWidth - box_width) / 2,
    (constants::screenHeight - box_height) / 2};

  int selected_idx_      = {-1};
  double last_input_     = {0};
  const double cooldown_ = {0.2};

private:
  void CreateButtons();
  void UpdateButtons();
  void DrawButtons();

public:
  ~PauseScene();
  PauseScene();

  void Init() override;
  void Update() override;
  void Draw() override;
  void Resume() override;
  SceneType Type() override;
};