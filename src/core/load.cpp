#include <raylib.h>
#include <string>

#include "include/core/application.hpp"
#include "include/core/button.hpp"
#include "include/core/character_selector.hpp"
#include "include/core/file_handler.hpp"
#include "include/core/game.hpp"
#include "include/core/load.hpp"

void LoadScene::Init() {
  buttons_.clear();
  buttons_.reserve(5);
  Rectangle src = {0, 0, 250, 141};
  buttons_.emplace_back(
    "Map 1",
    [this]() {
      LoadUsingMapPath("res/maps/map1.json");
    },
    src, buttonRects_[0], "res/ui/load_lvl1.png");
  buttons_.emplace_back(
    "Map 2",
    [this]() {
      LoadUsingMapPath("res/maps/map2.json");
    },
    src, buttonRects_[1], "res/ui/load_lvl2.png");
  buttons_.emplace_back(
    "Map 3",
    [this]() {
      LoadUsingMapPath("res/maps/map3.json");
    },
    src, buttonRects_[2], "res/ui/load_lvl3.png");
  buttons_.emplace_back(
    "Other",
    [this]() {
      std::string path = FileHandler::OpenFile();
      if (path.empty() != true)
        LoadUsingMapPath(path);
    },
    src, buttonRects_[3], "res/ui/load_lvl4.png");

  src = {0, 0, 16, 16};

  Rectangle dst = {32, 32, 64, 64};
  buttons_.emplace_back(
    "Return",
    []() {
      App.RemoveScene();
    },
    src, dst, "res/ui/buttons/return.png");
}

void LoadScene::Update() {
  if (IsKeyPressed(KEY_ESCAPE)) {
    App.RemoveScene();
    return;
  }

  double time = GetTime();
  if (IsKeyPressed(KEY_TAB) && (time - last_input_ >= cooldown_)) {
    if (selected_idx_ != -1)
      buttons_[selected_idx_].ToggleHighlight();
    selected_idx_ = (selected_idx_ + 1) % static_cast<int>(buttons_.size());
    buttons_[selected_idx_].ToggleHighlight();
  } else if (IsKeyPressed(KEY_ENTER) && selected_idx_ != -1)
    buttons_[selected_idx_].Activate();

  UpdateButtons();
}

void LoadScene::Draw() {
  const char *title = "Choose map";
  DrawText(title, 1024 / 2 - MeasureText(title, 80) / 2, 40, 80, BLACK);

  Font font = GetFontDefault();
  DrawButtons();
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
  for (auto &button : buttons_)
    button.Draw();
}

void LoadScene::UpdateButtons() {
  for (auto &button : buttons_) {
    if (button.Hovered()) {
      if (selected_idx_ != -1)
        buttons_[selected_idx_].ToggleHighlight();
      selected_idx_ = -1;
    }
    button.Update();
  }
}
