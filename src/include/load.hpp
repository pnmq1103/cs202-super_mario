#pragma once

#include "file_handler.hpp"
#include "scene.hpp"
#include <string>
#include <vector>

class LoadScene : public Scene {
public:
  LoadScene();
  ~LoadScene() override;
  void Init() override;
  void Update() override;
  void Draw() override;

private:
  std::string path;
  Rectangle rects[3];
  SaveDatawMap saveData[3];
  const int buttonWidth  = 256;
  const int buttonHeight = 128;
};