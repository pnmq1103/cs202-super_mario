#pragma once
#include <string>
#include <vector>

#include "include/core/scene.hpp"

class LoadScene : public Scene {
private:
  SceneType type_           = {Load};
  Rectangle buttonRects_[4] = {
    {400, 150, 250, 141}, // Button 1
    {400, 350, 250, 141}, // Button 2
    {400, 550, 250, 141}, // Button 3
    {400, 750, 250, 141}};
  std::vector<Button> buttons_;

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