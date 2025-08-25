#pragma once
#include "include/core/button.hpp"
#include "include/core/scene.hpp"
#include <cmath>

class CreditScene : public Scene {
private:
  SceneType type_ = {Credit};

  std::vector<Button> buttons_;

public:
  CreditScene();
  ~CreditScene();

  void Init() override;
  void Update() override;
  void Draw() override;
  void Resume() override;
  SceneType Type() override;
};