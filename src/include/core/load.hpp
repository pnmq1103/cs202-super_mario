#pragma once
#include <string>
#include <vector>

#include "include/core/scene.hpp"

class LoadScene : public Scene {
private:
  SceneType type_ = {Load};

  Rectangle buttonRects_[4] = {
    {400, 150, 250, 141},
    {400, 350, 250, 141},
    {400, 550, 250, 141},
    {400, 750, 250, 141}};
  std::vector<Button> buttons_;

  int selected_idx_      = {-1};
  double last_input_     = {0};
  const double cooldown_ = {0.2};

private:
  void LoadUsingMapPath(const std::string &mapPath);
  void Pause();
  void DrawButtons();
  void UpdateButtons();

public:
  LoadScene()           = default;
  ~LoadScene() override = default;

  void Init() override;
  void Update() override;
  void Draw() override;
  void Resume() override;
  SceneType Type() override;
};