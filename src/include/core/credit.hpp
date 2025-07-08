#pragma once
#include "scene.hpp"

class CreditScene : public Scene {
private:
  SceneType type_ = {Credit};

  Texture background_ = {};
  float timer_        = {0};
  float duration_     = {1};

public:
  CreditScene();
  ~CreditScene();

  void Init() override;
  void Update() override;
  void Draw() override;
  SceneType Type() override;
};