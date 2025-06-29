#pragma once

#include "scene.hpp"

class GameScene : public Scene {
private:
public:
  GameScene();
  ~GameScene();

  void Init() override;
  void Update() override;
  void Draw() override;
};
