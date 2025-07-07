#pragma once

#include "button.hpp"
#include "scene.hpp"
#include "slider.hpp"

class SettingScene : public Scene {
private:
  std::vector<Button> buttons_;
  std::vector<Slider> sliders_;
  float volume_ = {1};

public:
  SettingScene();
  ~SettingScene();

  void Init() override;
  void Update() override;
  void Draw() override;

  void CreateButtons();
  void UpdateButtons();
  void DrawButtons();

  void CreateSliders();
  void UpdateSliders();
  void DrawSliders();
};
