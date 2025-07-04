#pragma once

#include "button.hpp"
#include "scene.hpp"

class SettingScene : public Scene {
private:
public:
  SettingScene();
  ~SettingScene();

  void Init() override;
  void Update() override;
  void Draw() override;

  void CreateButtons();
};
