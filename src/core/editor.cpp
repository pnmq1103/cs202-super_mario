#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <raylib.h>
#include <raymath.h>
#include <stdexcept>

#include "../../tinyfiledialogs.h"
#include "../include/core/application.hpp"
#include "../include/core/editor.hpp"

EditorScene::EditorScene() {
  // The world position the camera_ is looking at (center of the view)
  camera_.target = {0, 0};
  // The screen position where the target will appear
  camera_.offset   = {0, 0};
  camera_.rotation = 0;
  camera_.zoom     = 1.0f;

  // Border + buffer to prevent flickering
  float buffer = blockSize / 4.0f;
  boundary_    = {
    0.0f + buffer, 0.0f + buffer, blockSize * block_width_ - 2 * buffer,
    blockSize * block_height_ - 2 * buffer};

  tilemap_.resize(block_width_ * block_height_, 0);
}

EditorScene::~EditorScene() {
  UnloadTexture(crosshair_);
  UnloadTexture(ground_tiles_);
}

void EditorScene::Init() {
  crosshair_    = LoadTexture("res/sprites/crosshairs/crosshair028.png");
  ground_tiles_ = LoadTexture("res/sprites/tilesets/tileset_ground.png");

  Scene::ReadSpriteInfo(
    "res/sprites/tilesets/tileset_ground.txt", ground_tiles_info_);
}

void EditorScene::Update() {
  if (IsKeyPressed(KEY_ESCAPE)) {
    Application::ChangeScene(nullptr);
    return;
  }

  // Drag
  if (IsMouseButtonDown(MOUSE_BUTTON_MIDDLE)) {
    Vector2 delta  = GetMouseDelta();
    delta          = Vector2Scale(delta, -1.0f / camera_.zoom);
    camera_.target = Vector2Add(camera_.target, delta);
  }

  mouse_world_pos_ = GetScreenToWorld2D(GetMousePosition(), camera_);

  // Snapping grid, update only if mouse is in the boundary
  if (CheckCollisionPointRec(mouse_world_pos_, boundary_)) {
    snapped_.x = std::floor(mouse_world_pos_.x / blockSize) * blockSize;
    snapped_.y = std::floor(mouse_world_pos_.y / blockSize) * blockSize;
  }

  PlaceBlock();
}

void EditorScene::Draw() {
  DrawGrid();
  DrawMap();
  DrawCursor();
}

void EditorScene::PlaceBlock() {
  if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
    Application::GetInstance().GetMedia().PlaySound("beep");

    float tile_x = std::floor(mouse_world_pos_.x / blockSize);
    float tile_y = std::floor(mouse_world_pos_.y / blockSize);
    if (CheckCollisionPointRec(mouse_world_pos_, boundary_)) {
      int idx = static_cast<int>(tile_y * block_width_ + tile_x);

      tilemap_[idx] = 1; // Change here for other sprites
    }
  } else if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
    float tile_x = std::floor(mouse_world_pos_.x / blockSize);
    float tile_y = std::floor(mouse_world_pos_.y / blockSize);
    if (CheckCollisionPointRec(mouse_world_pos_, boundary_)) {
      int idx = static_cast<int>(tile_y * block_width_ + tile_x);

      tilemap_[idx] = 0;
    }
  }
}

void EditorScene::DrawGrid() {
  Vector2 camera_topleft = {camera_.target.x - camera_.offset.x, 0.0f};

  int start_x = static_cast<int>(std::max(0.0f, camera_topleft.x / blockSize));
  int end_x   = static_cast<int>(Clamp(
    std::ceil((camera_topleft.x + screenWidth) / blockSize), 0.0f,
    static_cast<float>(block_width_)));

  BeginMode2D(camera_);
  for (int i = start_x; i <= end_x; ++i)
    DrawLineV({i * blockSize, 0.0f}, {i * blockSize, screenHeight}, LIGHTGRAY);

  for (int i = 0; i <= screenHeight / blockSize; ++i)
    DrawLineV(
      {start_x * blockSize, i * blockSize}, {end_x * blockSize, i * blockSize},
      LIGHTGRAY);
  EndMode2D();
}

void EditorScene::DrawCursor() {
  Color transparent = {128, 128, 128, 128};

  BeginMode2D(camera_);
  // Draw tile
  DrawTexturePro(
    ground_tiles_, ground_tiles_info_[59],
    {snapped_.x, snapped_.y, blockSize, blockSize}, {0, 0}, 0, transparent);
  // Draw crosshair
  DrawTexturePro(
    crosshair_, {0, 0, 64, 64}, {snapped_.x, snapped_.y, blockSize, blockSize},
    {0, 0}, 0, transparent);
  EndMode2D();
}

void EditorScene::DrawMap() {
  BeginMode2D(camera_);
  for (int y = 0; y < block_height_; ++y) {
    for (int x = 0; x < block_width_; ++x) {
      int idx = y * block_width_ + x;
      if (tilemap_[idx] == 1) {
        DrawTexturePro(
          ground_tiles_, ground_tiles_info_[59],
          {x * blockSize, y * blockSize, blockSize, blockSize}, {0, 0}, 0,
          WHITE);
      }
    }
  }
  EndMode2D();
}

void EditorScene::LoadFile() {
  const char *filter[]  = {"*.txt"};
  const char *file_path = tinyfd_openFileDialog(
    "Select map",
    "", // (last used folder)
    1, filter, "Text file (*.txt)", 0);

  if (file_path) {
    std::cout << "Trying to open " << file_path << '\n';
    std::ifstream fin(file_path);
    if (fin.is_open()) {
    } else {
      throw std::runtime_error("invalid file");
    }
    fin.close();
  }
}
