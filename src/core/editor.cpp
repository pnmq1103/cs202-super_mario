#include <cmath>
#include <fstream>
#include <iostream>
#include <raylib.h>
#include <raymath.h>
#include <stdexcept>

#include "include/core/application.hpp"
#include "include/core/constants.hpp"
#include "include/core/editor.hpp"
#include "include/core/enemy_selector.hpp"
#include "include/core/file_handler.hpp"
#include "include/core/tile_selector.hpp"

EditorScene::EditorScene() {
  camera_.target   = {0, 0};
  camera_.offset   = {0, 0};
  camera_.rotation = 0;
  camera_.zoom     = 1; // Important

  // Border + buffer to prevent flickering
  float buffer = constants::blockSize / 4;
  boundary_    = {
    0 + buffer, 0 + buffer,
    constants::blockSize * constants::mapWidth - 2 * buffer,
    constants::blockSize * constants::mapHeight - 2 * buffer};
}

EditorScene::~EditorScene() {
  UnloadTexture(crosshair_);
}

void EditorScene::Init() {
  crosshair_ = LoadTexture("res/sprites/crosshairs/crosshair028.png");
  map_.Init();
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
    snap_.x = std::floor(mouse_world_pos_.x / constants::blockSize)
              * constants::blockSize;
    snap_.y = std::floor(mouse_world_pos_.y / constants::blockSize)
              * constants::blockSize;
  }

  if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && IsKeyDown(KEY_SPACE)) {
    float col = std::floor(mouse_world_pos_.x / constants::blockSize);
    float row = std::floor(mouse_world_pos_.y / constants::blockSize);
    if (Vector2Equals(drag_delta_, {0, 0}) == false) {
      drag_delta_ = Vector2Subtract({col, row}, drag_delta_);
      int dx      = static_cast<int>(drag_delta_.x);
      int dy      = static_cast<int>(drag_delta_.y);
      int grid_cols
        = static_cast<int>(map_.GetTexture(1).width / constants::cellSize);
      int idx = static_cast<int>(dy * grid_cols + dx);

      select_gidx_ += idx;
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

void EditorScene::Resume() {
  button_clicked_ = false;
}

SceneType EditorScene::Type() {
  return type_;
}

void EditorScene::PlaceBlock() {
  float col = std::floor(mouse_world_pos_.x / constants::blockSize);
  float row = std::floor(mouse_world_pos_.y / constants::blockSize);
  int pos   = static_cast<int>(row * constants::mapWidth + col);

  if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
    if (CheckCollisionPointRec(mouse_world_pos_, boundary_)) {
      if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        App.Media().PlaySound("beep");

      map_.SetTile(pos, select_gidx_);
    }
  } else if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
    if (CheckCollisionPointRec(mouse_world_pos_, boundary_))
      map_.SetTile(pos, 0);
  }
}

void EditorScene::DrawGrid() {
  Vector2 camera_topleft = {
    camera_.target.x - camera_.offset.x, camera_.target.y - camera_.offset.y};

  int start_x
    = static_cast<int>(std::max(0.0f, camera_topleft.x / constants::blockSize));
  int end_x = static_cast<int>(Clamp(
    std::ceil(
      (camera_topleft.x + constants::screenWidth) / constants::blockSize),
    0, static_cast<float>(constants::mapWidth)));

  int start_y
    = static_cast<int>(std::max(0.0f, camera_topleft.y / constants::blockSize));
  int end_y = static_cast<int>(Clamp(
    std::ceil(
      (camera_topleft.y + constants::screenHeight) / constants::blockSize),
    0, static_cast<float>(constants::mapHeight)));

  BeginMode2D(camera_);
  // Draw vertical
  for (int i = start_x; i <= end_x; ++i)
    DrawLineV(
      {constants::blockSize * i, constants::blockSize * start_y},
      {constants::blockSize * i, constants::blockSize * end_y}, LIGHTGRAY);
  // Draw horizontal
  for (int i = start_y; i <= end_y; ++i)
    DrawLineV(
      {constants::blockSize * start_x, constants::blockSize * i},
      {constants::blockSize * end_x, constants::blockSize * i}, LIGHTGRAY);
  EndMode2D();
}

void EditorScene::DrawMap() {
  BeginMode2D(camera_);
  for (int y = 0; y < constants::mapHeight; ++y) {
    for (int x = 0; x < constants::mapWidth; ++x) {
      int gidx = map_.GetTile(y * constants::mapWidth + x);
      if (gidx != 0) {
        DrawTexturePro(
          map_.GetTexture(gidx), map_.GetInfo(gidx),
          {x * constants::blockSize, y * constants::blockSize,
           constants::blockSize, constants::blockSize},
          {0, 0}, 0, WHITE);
      }
    }
  }
  EndMode2D();
}

void EditorScene::DrawCursor() {
  Color transparent = {128, 128, 128, 128};

  BeginMode2D(camera_);
  //  Draw tile
  if (select_gidx_ != 0) {
    Rectangle src       = map_.GetInfo(select_gidx_);
    float scaled_width  = src.width * 4;
    float scaled_height = src.height * 4;
    Rectangle dst       = {
      snap_.x,
      snap_.y + constants::blockSize - scaled_height,
      scaled_width,
      scaled_height,
    };
    DrawTexturePro(
      map_.GetTexture(select_gidx_), src, dst, {0, 0}, 0, transparent);
  }

  // Draw crosshair
  // DrawTexturePro(
  //  crosshair_, {0, 0, 64, 64}, {snap_.x, snap_.y, constants::blockSize,
  //  constants::blockSize}, {0, 0}, 0, transparent);
  EndMode2D();
}

void EditorScene::CreateButtons() {
  buttons_.reserve(3);
  Rectangle source = {0, 0, 16, 16};
  Rectangle dest   = {100, 100, 64, 64};
  float spacing    = 30;

  // Choose tile
  buttons_.emplace_back(
    "Tilset",
    [this] {
      App.ChangeScene(std::make_unique<TileSelectorScene>(select_gidx_));
    },
    source, dest, "res/sprites/buttons/choose_ground_tile.png");

  // Choose enemy
  dest = {100 + 64 + spacing, 100, 64, 64};
  buttons_.emplace_back(
    "Enemy",
    [this] {
      App.ChangeScene(std::make_unique<EnemySelectorScene>(select_gidx_));
    },
    source, dest, "res/sprites/buttons/choose_underground_tile.png");

  // Save
  dest = {100 + 2 * (64 + spacing), 100, 64, 64};
  buttons_.emplace_back(
    "Save",
    [this] {
      FileHandler::SaveMapToFile(map_);
    },
    source, dest, "res/sprites/buttons/save.png");
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
