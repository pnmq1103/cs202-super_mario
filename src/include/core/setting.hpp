#pragma once
#include "include/core/button.hpp"
#include "include/core/scene.hpp"
#include "include/core/slider.hpp"

class SettingScene : public Scene {
private:
  SceneType type_ = {Setting};

  Texture background_ = {};
  std::vector<Button> buttons_;
  std::vector<Slider> sliders_;
  float volume_ = {1};

  float timer_    = {0};
  float duration_ = {1};

public:
  SettingScene();
  ~SettingScene();

  void Init() override;
  void Update() override;
  void Draw() override;
  SceneType Type() override;

  void CreateButtons();
  void UpdateButtons();
  void DrawButtons();

  void CreateSliders();
  void UpdateSliders();
  void DrawSliders();
};
