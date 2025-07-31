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

EditorScene::~EditorScene() {}

void EditorScene::Init() {
  map_.Init();
  CreateButtons();
}

void EditorScene::Update() {
  if (IsKeyPressed(KEY_ESCAPE)) {
    App.ChangeScene(nullptr);
    return;
  }

  UpdateCamera();
  UpdateMouse();
  UpdateButtons();
  if (button_clicked_ == false)
    PlaceBlock();
  UpdateShortkeys();

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
}

void EditorScene::Draw() {
  DrawGrid();
  DrawMap();
  DrawObjects();
  DrawPipe();
  DrawCursor();
  DrawButtons();
}

void EditorScene::Resume() {
  button_clicked_ = false;
}

SceneType EditorScene::Type() {
  return type_;
}

void EditorScene::CreateButtons() {
  buttons_.reserve(3);
  Rectangle source = {0, 0, 16, 16};
  Rectangle dest   = {100, 100, 64, 64};
  float spacing    = 40;

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
    source, dest, "res/sprites/buttons/choose_enemy.png");

  // Save
  dest = {100 + 2 * (64 + spacing), 100, 64, 64};
  buttons_.emplace_back(
    "Save",
    [this] {
      FileHandler::SaveMapToFile(map_);
    },
    source, dest, "res/sprites/buttons/save.png");
}

void EditorScene::UpdateCamera() {
  // Drag
  if (IsMouseButtonDown(MOUSE_BUTTON_MIDDLE)) {
    Vector2 delta  = Vector2Scale(GetMouseDelta(), -1 / camera_.zoom);
    camera_.target = Vector2Add(camera_.target, delta);
  }

  // Zoom
  camera_.zoom = expf(logf(camera_.zoom) + GetMouseWheelMove() * 0.1f);
  Clamp(camera_.zoom, 0.1f, 3.0f);
  if (IsKeyPressed(KEY_R))
    camera_.zoom = 1;
}

void EditorScene::UpdateMouse() {
  mouse_world_pos_ = GetScreenToWorld2D(GetMousePosition(), camera_);

  // Snapping grid, check only if mouse is in the boundary
  if (CheckCollisionPointRec(mouse_world_pos_, boundary_)) {
    snap_.x = std::floor(mouse_world_pos_.x / constants::blockSize);
    snap_.y = std::floor(mouse_world_pos_.y / constants::blockSize);
  }
}

void EditorScene::UpdateButtons() {
  for (size_t i = 0; i < buttons_.size(); ++i) {
    buttons_[i].Update();
    if (buttons_[i].Clicked())
      button_clicked_ = true;
  }
}

void EditorScene::UpdateShortkeys() {
  if (IsKeyPressed(KEY_T))
    App.ChangeScene(std::make_unique<TileSelectorScene>(select_gidx_));
  else if (IsKeyPressed(KEY_E))
    App.ChangeScene(std::make_unique<EnemySelectorScene>(select_gidx_));

  if (IsKeyPressed(KEY_TAB)) {
    int layer_idx
      = (static_cast<int>(cur_layer_) + 1) % static_cast<int>(MapLayer::Count);
    cur_layer_ = static_cast<MapLayer>(layer_idx);
  }
}

void EditorScene::PlaceBlock() {
  float col = std::floor(mouse_world_pos_.x / constants::blockSize);
  float row = std::floor(mouse_world_pos_.y / constants::blockSize);
  int pos   = static_cast<int>(row * constants::mapWidth + col);

  if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
    if (CheckCollisionPointRec(mouse_world_pos_, boundary_)) {
      if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        App.Media().PlaySound("beep");

      switch (cur_layer_) {
        case MapLayer::Tile1:
          map_.SetTile(pos, select_gidx_);
          break;
        case MapLayer::Objects:
          map_.SetEnemy(pos, select_gidx_);
          break;
        case MapLayer::Tile2:
          map_.SetPipe(pos, select_gidx_);
          break;
      }
    }
  } else if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
    if (CheckCollisionPointRec(mouse_world_pos_, boundary_))
      switch (cur_layer_) {
        case MapLayer::Tile1:
          map_.SetTile(pos, 0);
          break;
        case MapLayer::Objects:
          map_.SetEnemy(pos, 0);
          break;
        case MapLayer::Tile2:
          map_.SetPipe(pos, 0);
          break;
      }
  }
}

// Need to fix the optimization
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
      if (gidx != 0)
        DrawTexturePro(
          map_.GetTexture(gidx), map_.GetInfo(gidx),
          {x * constants::blockSize, y * constants::blockSize,
           constants::blockSize, constants::blockSize},
          {0, 0}, 0, WHITE);
    }
  }
  EndMode2D();
}

void EditorScene::DrawObjects() {
  BeginMode2D(camera_);
  for (int y = 0; y < constants::mapHeight; ++y) {
    for (int x = 0; x < constants::mapWidth; ++x) {
      int gidx = map_.GetEnemy(y * constants::mapWidth + x);
      if (gidx != 0) {
        Rectangle src       = map_.GetInfo(gidx);
        float scaled_width  = src.width * 4;
        float scaled_height = src.height * 4;
        Rectangle dst       = {
          x * constants::blockSize,
          (y + 1) * constants::blockSize - scaled_height, scaled_width,
          scaled_height};
        DrawTexturePro(map_.GetTexture(gidx), src, dst, {0, 0}, 0, WHITE);
      }
    }
  }
  EndMode2D();
}

void EditorScene::DrawPipe() {
  BeginMode2D(camera_);
  for (int y = 0; y < constants::mapHeight; ++y) {
    for (int x = 0; x < constants::mapWidth; ++x) {
      int gidx = map_.GetPipe(y * constants::mapWidth + x);
      if (gidx != 0)
        DrawTexturePro(
          map_.GetTexture(gidx), map_.GetInfo(gidx),
          {x * constants::blockSize, y * constants::blockSize,
           constants::blockSize, constants::blockSize},
          {0, 0}, 0, WHITE);
    }
  }
  EndMode2D();
}

void EditorScene::DrawButtons() {
  for (size_t i = 0; i < buttons_.size(); ++i)
    buttons_[i].Draw();
}

void EditorScene::DrawCursor() {
  Color transparent = {128, 128, 128, 128};

  BeginMode2D(camera_);
  if (select_gidx_ != 0) {
    Rectangle src       = map_.GetInfo(select_gidx_);
    float scaled_width  = src.width * 4;
    float scaled_height = src.height * 4;
    Rectangle dst       = {
      snap_.x * constants::blockSize,
      (snap_.y + 1) * constants::blockSize - scaled_height,
      scaled_width,
      scaled_height,
    };
    DrawTexturePro(
      map_.GetTexture(select_gidx_), src, dst, {0, 0}, 0, transparent);
  }
  EndMode2D();
}
