#include <raylib.h>
#include <string>

#include "include/core/application.hpp"
#include "include/core/button.hpp"
#include "include/core/character_selector.hpp"
#include "include/core/file_handler.hpp"
#include "include/core/game.hpp"
#include "include/core/load.hpp"

LoadScene::LoadScene() {}

LoadScene::~LoadScene() = default;

void LoadScene::Init() {}

void LoadScene::Update() {
  if (IsKeyPressed(KEY_ESCAPE)) {
    App.RemoveScene();
    return;
  }
  if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
    const Vector2 m = GetMousePosition();

    if (CheckCollisionPointRec(m, buttonRects_[0]))
      LoadUsingMapPath("res/maps/map1.json");
    else if (CheckCollisionPointRec(m, buttonRects_[1]))
      LoadUsingMapPath("res/maps/map2.json");
    else if (CheckCollisionPointRec(m, buttonRects_[2]))
      LoadUsingMapPath("res/maps/map3.json");
    else if (CheckCollisionPointRec(m, buttonRects_[3])) {
      std::string path = FileHandler::OpenFile();
      if (path.empty() != true)
        LoadUsingMapPath(path);
    }
  }
}

void LoadScene::Draw() {
  DrawText(
    "Choose map", 1024 / 2 - MeasureText("Choose map", 40) / 2, 20, 40, BLACK);

  for (int i = 0; i < 4; i++) {
    DrawRectangleRec(buttonRects_[i], LIGHTGRAY);
    int x = static_cast<int>(buttonRects_[i].x + (buttonRects_[i].width / 4));
    int y = static_cast<int>(buttonRects_[i].y + (buttonRects_[i].height / 4));
    if (i != 3)
      DrawText(TextFormat("Map %d", i + 1), x, y, 40, BLACK);
    else
      DrawText("Others", x, y, 40, BLACK);
  }
}

void LoadScene::Resume() {}

SceneType LoadScene::Type() {
  return type_;
}

void LoadScene::Pause() {}

void LoadScene::LoadUsingMapPath(const std::string &mapPath) {
  // default with luigi because game resets to luigi
  auto game
    = std::make_unique<GameScene>(CharacterSelectorScene::GetCharacterType());
  game->SetLevelPath(mapPath);
  App.AddScene(std::move(game));
}