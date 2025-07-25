#pragma once
#include <string>
#include <vector>

#include "include/core/scene.hpp"

class LoadScene : public Scene {
private:
  SceneType type_ = {Load};

  std::string path;
  Rectangle rects[3] = {};
  // SaveData saveData[3]   = {};
  const int buttonWidth  = {256};
  const int buttonHeight = {128};

public:
  LoadScene();
  ~LoadScene() override;

  void Init() override;
  void Update() override;
  void Draw() override;
  void Resume() override;
  SceneType Type() override;
};