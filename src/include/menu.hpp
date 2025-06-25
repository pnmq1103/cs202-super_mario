#pragma once
#include <raylib.h>

#include "scene.hpp"

class MenuScene : public Scene {
private:
  Texture background_;

public:
  MenuScene();
  ~MenuScene();

  void Init() override;
  void Update() override;
  void Draw() override;
};
