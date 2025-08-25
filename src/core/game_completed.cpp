#include "include/core/game_completed.hpp"

void GameCompletedScene::Init() {
  Image game_completed_image = LoadImage("res/game_completed.png");
  game_completed             = LoadTextureFromImage(game_completed_image);
  App.Media().PlayMusic("level_end");
}

void GameCompletedScene::Update() {
  //timer += GetTime();
  if (IsKeyPressed(KEY_ESCAPE) || timer >= 100.0f) {
    App.RemoveScene();
    return;
  }
}

void GameCompletedScene::Draw() {
  DrawTexture(game_completed, 0, 0, RAYWHITE);
}

void GameCompletedScene::Resume() {}

SceneType GameCompletedScene::Type() {
  return SceneType::GameCompleted;
}

