#pragma once
#include <string>
#include <vector>

#include "file_handler.hpp"
#include "scene.hpp"

class LoadScene : public Scene {
public:
  LoadScene();
  ~LoadScene() override;
  void Init() override;
  void Update() override;
  void Draw() override;

private:
  Rectangle rects[3];
  SaveDatawMap saveData[3];
  const int buttonWidth  = 256;
  const int buttonHeight = 128;
};
