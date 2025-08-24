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

void LoadScene::Init() {
  buttons_.clear();
  buttons_.reserve(4);
  Rectangle src = {0, 0, 250, 141};
  buttons_.emplace_back(
    "Map 1",
    [this]() {
      LoadUsingMapPath("res/maps/map1.json");
    },
    src, buttonRects_[0], "res/load_lvl1.png");
  buttons_.emplace_back(
    "Map 2",
    [this]() {
      LoadUsingMapPath("res/maps/map2.json");
    },
    src, buttonRects_[1], "res/load_lvl2.png");
  buttons_.emplace_back(
    "Map 3",
    [this]() {
      LoadUsingMapPath("res/maps/map3.json");
    },
    src, buttonRects_[2], "res/load_lvl3.png");
  buttons_.emplace_back(
    "Other",
    [this]() {
      std::string path = FileHandler::OpenFile();
      if (path.empty() != true)
        LoadUsingMapPath(path);
    },
    src, buttonRects_[3], "res/load_lvl4.png");
}

void LoadScene::Update() {
  if (IsKeyPressed(KEY_ESCAPE)) {
    App.RemoveScene();
    return;
  }
  UpdateButtons();
  /*if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
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
  }*/
}

void LoadScene::Draw() {
  const char *title = "Choose map";
  DrawText(title, 1024 / 2 - MeasureText(title, 40) / 2, 20, 40, BLACK);

  Font font = GetFontDefault();
  DrawButtons();
  /*for (int i = 0; i < 4; i++) {
    DrawRectangleRec(buttonRects_[i], LIGHTGRAY);
    std::string text = (i != 3) ? "Map " + std::to_string(i + 1) : "Others";
    Vector2 sz       = MeasureTextEx(font, text.c_str(), 40, 1);

    float x = buttonRects_[i].x + (buttonRects_[i].width - sz.x) / 2;
    float y = buttonRects_[i].y + (buttonRects_[i].height - sz.y) / 2;

    DrawText(text.c_str(), static_cast<int>(x), static_cast<int>(y), 40, BLACK);
  }*/
}

void LoadScene::Resume() {}

SceneType LoadScene::Type() {
  return type_;
}

void LoadScene::Pause() {}

void LoadScene::LoadUsingMapPath(const std::string &mapPath) {
  App.AddScene(std::make_unique<CharacterSelectorScene>(mapPath));
}

void LoadScene::DrawButtons() {
  for (auto &button : buttons_) {
    button.Draw();
  }
}

void LoadScene::UpdateButtons() {
  for (auto &button : buttons_) {
    button.Update();
  }
}
