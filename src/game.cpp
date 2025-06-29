#include <raylib.h>

#include "include/application.hpp"
#include "include/game.hpp"

GameScene::GameScene() {}

GameScene::~GameScene() {}

void GameScene::Init() {}

void GameScene::Update() {
  if (IsKeyPressed(KEY_ESCAPE))
    Application::ChangeScene(nullptr);
}

void GameScene::Draw() {}
