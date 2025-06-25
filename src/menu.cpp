#include "include/menu.hpp"

MenuScene::MenuScene() : background_{} {}

MenuScene::~MenuScene() {}

void MenuScene::Init() { background_ = LoadTexture("res/menu_background.png"); }

void MenuScene::Update() {}

void MenuScene::Draw() {
  const int screenWidth = 256 * 4, screenHeight = 240 * 4;
  float x_coor = (screenWidth - background_.width) / 2.0f;
  float y_coor = (screenHeight - background_.height) / 2.0f;
  DrawTextureEx(background_, {x_coor, y_coor}, 0.0f, 1.0f, RAYWHITE);

  const char *title = "Mario";
  float font_size = 150;
  Vector2 title_size = MeasureTextEx(GetFontDefault(), title, font_size, 1.0f);
  x_coor = (screenWidth - title_size.x) / 2.0f;
  y_coor = (screenHeight - title_size.y) / 6.0f;
  DrawText(title, static_cast<int>(x_coor), static_cast<int>(y_coor),
           static_cast<int>(font_size), RAYWHITE);
}
