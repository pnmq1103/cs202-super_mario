#include <cmath>
#include <iostream>
#include <memory>
#include <raylib.h>
#include <stdexcept>

#include "include/application.hpp"
#include "include/editor.hpp"
#include "include/game.hpp"
#include "include/load.hpp"
#include "include/menu.hpp"
#include "include/setting.hpp"

MenuScene::MenuScene() {
  menu_items_.resize(4);
  menu_items_ = {"Play", "Load", "Editor", "Exit"};
  buttons_.reserve(1);
}

MenuScene::~MenuScene() {
  UnloadTexture(background_);
}

void MenuScene::Init() {
  background_ = LoadTexture("res/menu_background.png");
  Application::GetInstance().GetMedia().PlayMusic("title");

  Scene::ReadSpriteInfo(
    "res/sprites/user_interface/buttons.txt", buttons_info_);
  std::cout << buttons_info_[15].x << ' ' << buttons_info_[15].y << '\n';
  CreateButtons();
}

void MenuScene::Update() {
  int size = static_cast<int>(menu_items_.size());

  double time = GetTime();
  if (time - last_input_ >= cooldown_) {
    if (IsKeyDown(KEY_DOWN)) {
      last_input_   = time;
      selected_idx_ = (selected_idx_ + 1) % size;
    } else if (IsKeyDown(KEY_UP)) {
      last_input_   = time;
      selected_idx_ = (selected_idx_ - 1 + size) % size;
    } else if (IsKeyDown(KEY_ENTER)) {
      last_input_ = time;

      switch (selected_idx_) {
        case MenuOption::Game:
          Application::GetInstance().ChangeScene(std::make_unique<GameScene>());
          break;
        case MenuOption::Load:
          Application::GetInstance().ChangeScene(std::make_unique<LoadScene>());
          break;
        case MenuOption::Editor:
          Application::GetInstance().ChangeScene(
            std::make_unique<EditorScene>());
          break;
        case MenuOption::Exit:
          Application::GetInstance().Close();
          break;

        default:
          throw std::invalid_argument("Must be positive");
          break;
      }
    }
  }

  // Update buttons
  for (size_t i = 0; i < buttons_.size(); ++i)
    buttons_[i].Update();
}

void MenuScene::Draw() {
  // Draw background
  float x = (screenWidth - background_.width) / 2.0f;
  float y = (screenHeight - background_.height) / 2.0f;
  DrawTextureEx(background_, {x, y}, 0.0f, 1.0f, RAYWHITE);

  // Draw title
  float font_size    = 150;
  const char *title  = "Mario";
  Vector2 title_size = MeasureTextEx(GetFontDefault(), title, font_size, 1.0f);
  x                  = (screenWidth - title_size.x) / 2.0f;
  y                  = (screenHeight - title_size.y) / 6.0f;
  DrawTextEx(GetFontDefault(), title, {x, y}, font_size, 1.0f, RAYWHITE);

  // Draw options
  font_size = 40;
  for (size_t i = 0; i < menu_items_.size(); ++i) {
    const char *option = menu_items_[i].c_str();
    Vector2 option_size
      = MeasureTextEx(GetFontDefault(), option, font_size, 1.0f);
    x = (screenWidth - option_size.x) / 2.0f;
    y = (screenHeight - option_size.y) * (i + 12) / 16.0f;

    double time   = GetTime();
    float wave    = static_cast<float>((sin(time * 3.0) + 1.0) * 0.5); // [0, 1]
    float pulsing = 0.1f * wave;
    float scale   = 1.0f;
    float opacity = 0.5f + 0.5f * wave;
    Color color   = RAYWHITE;

    if (i == selected_idx_) {
      scale = 1.0f + pulsing;
      color = {245, 245, 245, static_cast<unsigned char>(255 * opacity)};

      // Draw arrow
      float space  = 20.0f;
      float length = option_size.y * scale;
      float angle  = static_cast<float>(cos(PI / 6.0f));
      Vector2 a    = {x - space, y + length / 2.0f};
      Vector2 b    = {x - space - length * angle, y};
      Vector2 c    = {x - space - length * angle, y + length};

      DrawTriangle(a, b, c, color);
    }

    DrawTextEx(
      GetFontDefault(), option, {x, y}, font_size * static_cast<float>(scale),
      3.0f, color);
  }

  // Draw buttons
  for (size_t i = 0; i < buttons_.size(); ++i)
    buttons_[i].Draw();
}

void MenuScene::CreateButtons() {
  Rectangle dest = {screenWidth - 100, screenHeight - 100, 48, 48};
  Rectangle src  = {32, 48, 16, 16};
  buttons_.emplace_back(
    []() {
      Application::GetInstance().ChangeScene(std::make_unique<SettingScene>());
    },
    src, dest, "res/sprites/user_interface/buttons.png");
}
