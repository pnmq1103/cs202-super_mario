#include <raylib.h>
#include <raymath.h>

#include "include/core/application.hpp"
#include "include/core/scene.hpp"
#include "include/core/tile_selector.hpp"

TileSelectorScene::TileSelectorScene(int &selected_tile_id)
    : selected_tile_id_ref_(selected_tile_id) {
  camera_.target   = {0, 0};
  camera_.offset   = {0, 0};
  camera_.rotation = 0;
  camera_.zoom     = 1; // Important

  buttons_.reserve(4);
}

TileSelectorScene::~TileSelectorScene() {
  UnloadTexture(ground_tiles_);
}

void TileSelectorScene::Init() {
  ground_tiles_ = LoadTexture("res/sprites/tilesets/tileset_ground.png");
  Scene::ReadSpriteInfo(
    "res/sprites/tilesets/tileset_ground.txt", ground_tiles_info_);

  float width  = static_cast<float>(ground_tiles_.width);
  float height = static_cast<float>(ground_tiles_.height);
  boundary_    = {0, blockSize * 2, width * 4, height * 4};

  BuildSpriteGrid();
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
  float width  = static_cast<float>(ground_tiles_.width);
  float height = static_cast<float>(ground_tiles_.height);
  DrawRectangleRec(
    {blockSize * 2, blockSize / 2, blockSize * 5, blockSize}, GRAY);
  DrawTexturePro(
    ground_tiles_, {0, 0, width, height}, boundary_, {0, 0}, 0, WHITE);
  EndMode2D();
}

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
  camera_.target.x
    = Clamp(camera_.target.x, 0, boundary_.x + boundary_.width - screenWidth);
  camera_.target.y
    = Clamp(camera_.target.y, 0, boundary_.y + boundary_.height - screenHeight);
}

void TileSelectorScene::BuildSpriteGrid() {
  grid_cols_ = static_cast<int>(ground_tiles_.width / cellSize);
  grid_rows_ = static_cast<int>(ground_tiles_.height / cellSize);
  sprites_grid_.resize(grid_cols_ * grid_rows_);

  for (const auto &[sprite_id, bounds] : ground_tiles_info_) {
    int start_col = static_cast<int>(bounds.x / cellSize);
    int end_col   = static_cast<int>((bounds.x + bounds.width) / cellSize);
    int start_row = static_cast<int>(bounds.y / cellSize);
    int end_row   = static_cast<int>((bounds.y + bounds.height) / cellSize);

    for (int row = start_row; row <= end_row; ++row) {
      for (int col = start_col; col <= end_col; ++col) {
        if (row >= 0 && row < grid_rows_ && col >= 0 && col < grid_cols_)
          sprites_grid_[row * grid_cols_ + col].push_back(sprite_id);
      }
    }
  }
}

void TileSelectorScene::ChooseTile() {
  App.Media().PlaySound("beep");
  mouse_world_pos_.y -= blockSize * 2; // Compensate for UI
  // Because the display is scaled up 4 times
  mouse_world_pos_ = Vector2Scale(mouse_world_pos_, 0.25f);

  int col = static_cast<int>(mouse_world_pos_.x / cellSize);
  int row = static_cast<int>(mouse_world_pos_.y / cellSize);

  if (row >= 0 && row < grid_rows_ && col >= 0 && col < grid_cols_) {
    for (int sprite_idx : sprites_grid_[row * grid_cols_ + col]) {
      const Rectangle &sprite_bounds = ground_tiles_info_[sprite_idx];
      if (CheckCollisionPointRec(mouse_world_pos_, sprite_bounds)) {
        selected_tile_id_ref_ = sprite_idx;
        break;
      }
    }
  }
}
