#pragma once

#include "FileHandler.hpp"
#include "scene.hpp"
#include <vector>
#include <string>

class LoadScene : public Scene {
public:
  LoadScene();
  ~LoadScene() override;
  void Init() override;
  void Update() override;
  void Draw() override;

private:
  Rectangle rects[3];
  SaveData saveData[3];
  const int buttonWidth          = 256;
  const int buttonHeight         = 128;
};
