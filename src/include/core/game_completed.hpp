#pragma once

#include "include/core/scene.hpp"
#include "include/core/application.hpp"

class GameCompletedScene : public Scene {
  Texture game_completed;
  float timer = 0.0f;

public:
  GameCompletedScene() = default;
  ~GameCompletedScene() override = default;
  void Init() override;
  void Update() override;
  void Draw() override;
  void Resume() override;
  SceneType Type() override;
};