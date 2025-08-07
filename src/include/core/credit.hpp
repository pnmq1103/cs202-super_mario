#pragma once
#include "include/core/scene.hpp"
#include <cmath>

class CreditScene : public Scene {
private:
  SceneType type_ = {Credit};

public:
  CreditScene();
  ~CreditScene();

  void Init() override;
  void Update() override;
  void Draw() override;
  void Resume() override;
  SceneType Type() override;
};