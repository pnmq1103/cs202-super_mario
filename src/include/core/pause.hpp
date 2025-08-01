#pragma once

#include "include/core/scene.hpp"

class PauseScene : public Scene {
private:
  SceneType type_ = {Pause};

public:
  void Init() override;
  void Update() override;
  void Draw() override;
  void Resume() override;
  SceneType Type() override;
};