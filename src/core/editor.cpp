#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <raylib.h>
#include <raymath.h>
#include <stdexcept>

#include "include/core/application.hpp"
#include "include/core/editor.hpp"
#include "include/core/tile_selector.hpp"

EditorScene::EditorScene() {
  camera_.target   = {0, 0};
  camera_.offset   = {0, 0};
  camera_.rotation = 0;
  camera_.zoom     = 1; // Important

  // Border + buffer to prevent flickering
  float buffer = blockSize / 4;
  boundary_    = {
    0 + buffer, 0 + buffer, blockSize * block_width_ - 2 * buffer,
    blockSize * block_height_ - 2 * buffer};

  tilemap_.resize(block_width_ * block_height_, 0);
  buttons_.reserve(3);
  sprite_sheets_.reserve(3);
  sprite_sheets_ = {
    {1, 10, App.Resource().GetBackground()},
    {11, 144, App.Resource().GetTileset('g')},
    {155, 144, App.Resource().GetTileset('u')},
  };
}

EditorScene::~EditorScene() {
  UnloadTexture(crosshair_);
}

void EditorScene::Init() {
  crosshair_ = LoadTexture("res/sprites/crosshairs/crosshair028.png");

  Scene::ReadSpriteInfo(
    "res/sprites/tilesets/tileset_ground.txt", tileset_info_);

  CreateButtons();
}

void EditorScene::Update() {
  if (IsKeyPressed(KEY_ESCAPE)) {
    App.ChangeScene(nullptr);
    return;
  }

  // Drag
  if (IsMouseButtonDown(MOUSE_BUTTON_MIDDLE)) {
    Vector2 delta  = Vector2Scale(GetMouseDelta(), -1 / camera_.zoom);
    camera_.target = Vector2Add(camera_.target, delta);
  }

  mouse_world_pos_ = GetScreenToWorld2D(GetMousePosition(), camera_);

  // Snapping grid, check only if mouse is in the boundary
  if (CheckCollisionPointRec(mouse_world_pos_, boundary_)) {
    snapped_.x = std::floor(mouse_world_pos_.x / blockSize) * blockSize;
    snapped_.y = std::floor(mouse_world_pos_.y / blockSize) * blockSize;
  }

  if (IsKeyDown(KEY_SPACE) && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
    float col = std::floor(mouse_world_pos_.x / blockSize);
    float row = std::floor(mouse_world_pos_.y / blockSize);
    if (Vector2Equals(drag_delta_, {0, 0}) == false) {
      drag_delta_   = Vector2Subtract({col, row}, drag_delta_);
      int dx        = static_cast<int>(drag_delta_.x);
      int dy        = static_cast<int>(drag_delta_.y);
      int grid_cols = static_cast<int>(tileset_->width / cellSize);
      int idx       = static_cast<int>(dy * grid_cols + dx);

      global_selected_idx_ += idx;
    }
    drag_delta_ = {col, row};
  } else if (IsKeyUp(KEY_SPACE))
    drag_delta_ = {0, 0};

  UpdateButtons();
  if (button_clicked_ == false)
    PlaceBlock();
}

void EditorScene::Draw() {
  DrawGrid();
  DrawMap();
  DrawCursor();
  DrawButtons();
}

SceneType EditorScene::Type() {
  return type_;
}

void EditorScene::Resume() {
  button_clicked_ = false;
}

void EditorScene::PlaceBlock() {
  float tile_x = std::floor(mouse_world_pos_.x / blockSize);
  float tile_y = std::floor(mouse_world_pos_.y / blockSize);
  int idx      = static_cast<int>(tile_y * block_width_ + tile_x);

  if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
    if (CheckCollisionPointRec(mouse_world_pos_, boundary_)) {
      App.Media().PlaySound("beep");
      tilemap_[idx] = global_selected_idx_;
    }
  } else if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
    if (CheckCollisionPointRec(mouse_world_pos_, boundary_))
      tilemap_[idx] = 0;
  }
}

void EditorScene::DrawGrid() {
  Vector2 camera_topleft = {
    camera_.target.x - camera_.offset.x, camera_.target.y - camera_.offset.y};

  int start_x = static_cast<int>(std::max(0.0f, camera_topleft.x / blockSize));
  int end_x   = static_cast<int>(Clamp(
    std::ceil((camera_topleft.x + screenWidth) / blockSize), 0,
    static_cast<float>(block_width_)));

  int start_y = static_cast<int>(std::max(0.0f, camera_topleft.y / blockSize));
  int end_y   = static_cast<int>(Clamp(
    std::ceil((camera_topleft.y + screenHeight) / blockSize), 0,
    static_cast<float>(block_height_)));

  BeginMode2D(camera_);
  // Draw vertical
  for (int i = start_x; i <= end_x; ++i)
    DrawLineV(
      {blockSize * i, blockSize * start_y}, {blockSize * i, blockSize * end_y},
      LIGHTGRAY);
  // Draw horizontal
  for (int i = start_y; i <= end_y; ++i)
    DrawLineV(
      {blockSize * start_x, blockSize * i}, {blockSize * end_x, blockSize * i},
      LIGHTGRAY);
  EndMode2D();
}

void EditorScene::DrawMap() {
  BeginMode2D(camera_);
  for (int y = 0; y < block_height_; ++y) {
    for (int x = 0; x < block_width_; ++x) {
      int global_idx = tilemap_[y * block_width_ + x];
      if (global_idx != 0) {
        int local_idx = FindLocalIndex(global_idx);
        DrawTexturePro(
          *tileset_, tileset_info_[local_idx],
          {x * blockSize, y * blockSize, blockSize, blockSize}, {0, 0}, 0,
          WHITE);
      }
    }
  }
  EndMode2D();
}

void EditorScene::DrawCursor() {
  Color transparent = {128, 128, 128, 128};

  BeginMode2D(camera_);
  //  Draw tile
  if (global_selected_idx_ != 0) {
    int local_idx = FindLocalIndex(global_selected_idx_);
    DrawTexturePro(
      *tileset_, tileset_info_[local_idx],
      {snapped_.x, snapped_.y, blockSize, blockSize}, {0, 0}, 0, transparent);
  }

  // Draw crosshair
  DrawTexturePro(
    crosshair_, {0, 0, 64, 64}, {snapped_.x, snapped_.y, blockSize, blockSize},
    {0, 0}, 0, transparent);
  EndMode2D();
}

void EditorScene::CreateButtons() {
  // Choose tile
  buttons_.emplace_back(
    "Overworld",
    [this] {
      App.ChangeScene(
        std::make_unique<TileSelectorScene>(global_selected_idx_));
    },
    Rectangle{0, 0, 16, 16}, Rectangle{100, 100, 64, 64},
    "res/sprites/buttons/choose_ground_tile.png");

  // Choose enemy
  // Implement EnemySelectorScene

  // Choose background
  // Implement BackgroundSelectorScene
}

void EditorScene::UpdateButtons() {
  for (size_t i = 0; i < buttons_.size(); ++i) {
    buttons_[i].Update();
    if (buttons_[i].Clicked())
      button_clicked_ = true;
  }
}

void EditorScene::DrawButtons() {
  for (size_t i = 0; i < buttons_.size(); ++i)
    buttons_[i].Draw();
}

int EditorScene::FindLocalIndex(int gidx) {
  for (const auto &sheet : sprite_sheets_) {
    if (gidx >= sheet.first_gidx_ && gidx < sheet.first_gidx_ + sheet.count_) {
      tileset_ = sheet.texture_;
      return gidx - sheet.first_gidx_;
    }
  }
  return -1;
}