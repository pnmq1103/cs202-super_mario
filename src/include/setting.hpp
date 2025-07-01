#pragma once

#include "scene.hpp"

class SettingScene : public Scene {
private:
public:
  SettingScene();
  ~SettingScene();

  void Init() override;
  void Update() override;
  void Draw() override;
};
