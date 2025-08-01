#include "include/core/pause.hpp"
#include "include/core/application.hpp"
#include "include/core/constants.hpp"

void PauseScene::Init() {}

void PauseScene::Update() {
  if (IsKeyPressed(KEY_ESCAPE))
    App.RemoveScene(2);
}

void PauseScene::Draw() {
  Vector2 center = {constants::screenWidth / 2, constants::screenHeight / 2};
  float width    = constants::screenWidth / 5;
  float height   = constants::screenHeight / 4;
  DrawRectangleRec(
    {0, 0, constants::screenWidth, constants::screenHeight},
    Fade(LIGHTGRAY, 0.9f));
  DrawRectangleRounded(
    {center.x - width / 2, center.y - height / 2, width, height}, 0.3f, 20,
    RED);
}

void PauseScene::Resume() {}

SceneType PauseScene::Type() {
  return type_;
}
