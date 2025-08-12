#pragma once
#include <string>
#include <vector>

#include "include/core/scene.hpp"

class LoadScene : public Scene {
private:
  SceneType type_           = {Load};
  Rectangle buttonRects_[4] = {
    {412, 200, 200, 100}, // Button 1
    {412, 400, 200, 100}, // Button 2
    {412, 600, 200, 100}, // Button 3
    {412, 800, 200, 100}};
  void LoadUsingMapPath(const std::string &mapPath);

public:
  LoadScene();
  ~LoadScene() override;

  void Init() override;
  void Update() override;
  void Draw() override;
  void Resume() override;
  SceneType Type() override;
  void Pause();
  void DrawButtons();
  void UpdateButtons();
};