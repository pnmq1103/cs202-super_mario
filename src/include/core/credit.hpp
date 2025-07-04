#pragma once
#include "scene.hpp"

class CreditScene : public Scene {
private:
public:
  CreditScene();
  ~CreditScene();

  void Init() override;
  void Update() override;
  void Draw() override;
};