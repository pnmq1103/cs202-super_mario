#include <raylib.h>
#include <raymath.h>

#include "include/core/application.hpp"
#include "include/core/constants.hpp"
#include "include/core/enemy_selector.hpp"
#include "include/core/scene.hpp"
#include "include/core/utility.hpp"

EnemySelectorScene::EnemySelectorScene(int &g_select_idx)
    : g_select_idx_ref_(g_select_idx) {
  camera_.target   = {0, 0};
  camera_.offset   = {0, 0};
  camera_.rotation = 0;
  camera_.zoom     = 1; // Important
}

EnemySelectorScene::~EnemySelectorScene() {}

void EnemySelectorScene::Init() {
  sprite_sheet_ = &App.Resource().GetEnemy();
  utility::ReadSpriteInfo(
    "res/sprites/enemies/enemies.txt", sprite_sheet_info_);

  float width  = static_cast<float>(sprite_sheet_->width);
  float height = static_cast<float>(sprite_sheet_->height);
  boundary_    = {0, constants::blockSize * 2, width * 4, height * 4};
  grid_cols_   = static_cast<int>(width / constants::cellSize);
  grid_rows_   = static_cast<int>(height / constants::cellSize);
}

void EnemySelectorScene::Update() {
  if (IsKeyDown(KEY_ESCAPE)) {
    App.ChangeScene(nullptr);
    return;
  }

  UpdateCamera();
  mouse_world_pos_ = GetScreenToWorld2D(GetMousePosition(), camera_);

  if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
    if (CheckCollisionPointRec(mouse_world_pos_, boundary_))
      ChooseTile();
  }
}

void EnemySelectorScene::Draw() {
  BeginMode2D(camera_);
  float width  = static_cast<float>(sprite_sheet_->width);
  float height = static_cast<float>(sprite_sheet_->height);
  DrawRectangleRec(
    {constants::blockSize * 2, constants::blockSize / 2,
     constants::blockSize * 5, constants::blockSize},
    GRAY);
  DrawTexturePro(
    *sprite_sheet_, {0, 0, width, height}, boundary_, {0, 0}, 0, WHITE);
  EndMode2D();
}

void EnemySelectorScene::Resume() {}

SceneType EnemySelectorScene::Type() {
  return type_;
}

void EnemySelectorScene::UpdateCamera() {
  // Drag
  if (IsMouseButtonDown(MOUSE_BUTTON_MIDDLE)) {
    Vector2 delta  = Vector2Scale(GetMouseDelta(), -1 / camera_.zoom);
    camera_.target = Vector2Add(camera_.target, delta);
  }

  // Scroll
  Vector2 scroll_offset_
    = Vector2Scale(GetMouseWheelMoveV(), -1 * scroll_speed_);
  camera_.target = Vector2Add(camera_.target, scroll_offset_);

  // Restrict camera
  camera_.target.x = Clamp(
    camera_.target.x, 0,
    boundary_.x + boundary_.width - constants::screenWidth);
  camera_.target.y = Clamp(
    camera_.target.y, 0,
    boundary_.y + boundary_.height - constants::screenHeight);
}

void EnemySelectorScene::ChooseTile() {
  if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    App.Media().PlaySound("beep");

  mouse_world_pos_.y -= constants::blockSize * 2; // Compensate for UI
  // Because the display is scaled up 4 times
  mouse_world_pos_ = Vector2Scale(mouse_world_pos_, 0.25f);

  int col = static_cast<int>(mouse_world_pos_.x / constants::cellSize);
  int row = static_cast<int>(mouse_world_pos_.y / constants::cellSize);

  if (row >= 0 && row < grid_rows_ && col >= 0 && col < grid_cols_)
    g_select_idx_ref_ = first_gidx + row * grid_cols_ + col;
}
