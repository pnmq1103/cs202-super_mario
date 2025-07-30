#include <raylib.h>
#include <raymath.h>

#include "include/core/application.hpp"
#include "include/core/constants.hpp"
#include "include/core/scene.hpp"
#include "include/core/tile_selector.hpp"

TileSelectorScene::TileSelectorScene(int &select_gidx)
    : select_gidx_ref_(select_gidx) {
  camera_.target   = {0, 0};
  camera_.offset   = {0, 0};
  camera_.rotation = 0;
  camera_.zoom     = 1; // Important

  buttons_.reserve(3);
}

TileSelectorScene::~TileSelectorScene() {}

void TileSelectorScene::Init() {
  sprite_sheet_ = &App.Resource().GetTileset('g');

  float width  = static_cast<float>(sprite_sheet_->width);
  float height = static_cast<float>(sprite_sheet_->height);
  boundary_    = {0, 0, width * 4, height * 4};
  grid_cols_   = static_cast<int>(width / constants::cellSize);
  grid_rows_   = static_cast<int>(height / constants::cellSize);
}

void TileSelectorScene::Update() {
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

void TileSelectorScene::Draw() {
  BeginMode2D(camera_);
  float width  = static_cast<float>(sprite_sheet_->width);
  float height = static_cast<float>(sprite_sheet_->height);
  DrawTexturePro(
    *sprite_sheet_, {0, 0, width, height}, boundary_, {0, 0}, 0, WHITE);
  EndMode2D();
}

void TileSelectorScene::Resume() {}

SceneType TileSelectorScene::Type() {
  return type_;
}

void TileSelectorScene::UpdateCamera() {
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
  float world_width  = boundary_.x + boundary_.width;
  float world_height = boundary_.y + boundary_.height;
  if (constants::screenWidth > world_width)
    camera_.target.x = (world_width - constants::screenWidth) / 2.0f;
  else
    camera_.target.x
      = Clamp(camera_.target.x, 0.0f, constants::screenWidth - world_width);
  if (constants::screenHeight > world_height)
    camera_.target.y = (world_height - constants::screenHeight) / 2.0f;
  else
    camera_.target.y
      = Clamp(camera_.target.y, 0.0f, constants::screenHeight - world_height);
}

void TileSelectorScene::ChooseTile() {
  if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    App.Media().PlaySound("beep");

  //  Because the display is scaled up 4 times
  mouse_world_pos_ = Vector2Scale(mouse_world_pos_, 0.25f);

  int col = static_cast<int>(mouse_world_pos_.x / constants::cellSize);
  int row = static_cast<int>(mouse_world_pos_.y / constants::cellSize);

  if (row >= 0 && row < grid_rows_ && col >= 0 && col < grid_cols_)
    select_gidx_ref_ = first_gidx + row * grid_cols_ + col;
}
