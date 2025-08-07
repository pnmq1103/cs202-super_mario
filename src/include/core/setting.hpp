#pragma once
#include "include/core/button.hpp"
#include "include/core/scene.hpp"
#include "include/core/slider.hpp"
#include <cmath>

class SettingScene : public Scene {
private:
  SceneType type_ = {Setting};

  std::vector<Button> buttons_;
  std::vector<Slider> sliders_;

private:
  void CreateButtons();
  void UpdateButtons();
  void DrawButtons();

  void CreateSliders();
  void UpdateSliders();
  void DrawSliders();

public:
  SettingScene();
  ~SettingScene();

  void Init() override;
  void Update() override;
  void Draw() override;
  void Resume() override;
  SceneType Type() override;
};

// if volume = 0->translucent and red diagonal line
