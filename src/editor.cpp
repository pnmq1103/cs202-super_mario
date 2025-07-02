#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <raylib.h>
#include <raymath.h>

#include "../tinyfiledialogs.h"
#include "include/application.hpp"
#include "include/editor.hpp"

EditorScene::EditorScene() {
  // The world position the camera_ is looking at (center of the view).
  camera_.target = {0.0f, 0.0f};

  // The screen position where the target will appear.
  camera_.offset   = {0.0f, 0.0f};
  camera_.rotation = 0.0f;
  camera_.zoom     = 1.0f;

  // Border + buffer to prevent flickering
  float bl_sz  = static_cast<float>(blockSize);
  float buffer = bl_sz / 4.0f;

  boundary_ = {
    0.0f + buffer, 0.0f + buffer, bl_sz * block_width_ - 2 * buffer,
    bl_sz * block_height_ - 2 * buffer};
}

EditorScene::~EditorScene() {
  UnloadTexture(ground_tiles_);
}

void EditorScene::Init() {
  ground_tiles_ = LoadTexture("res/sprites/tilesets/tileset_ground.png");

  std::ifstream fin;
  fin.open("res/sprites/tilesets/tileset_ground.txt");
  if (fin.is_open()) {
    int id;
    float x, y, w, h;
    while (fin >> id) {
      fin >> x >> y >> w >> h;
      ground_tiles_info_[id] = {x, y, w, h};
    }
  }
}

void EditorScene::Update() {
  if (IsKeyPressed(KEY_ESCAPE)) {
    Application::ChangeScene(nullptr);
    return;
  }

  if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
    Vector2 delta  = GetMouseDelta();
    delta          = Vector2Scale(delta, -1.0f / camera_.zoom);
    camera_.target = Vector2Add(camera_.target, delta);
  }

  if (IsKeyDown(KEY_O))
    LoadFile();
  if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
    Application::GetInstance().GetMedia().PlaySound("beep");
  }
}

void EditorScene::Draw() {
  DrawGrid();
  DrawCursor();
}

void EditorScene::DrawGrid() {
  float bl_sz = static_cast<float>(blockSize);

  Vector2 camera_topleft = {camera_.target.x - camera_.offset.x, 0.0f};
  int start_x = std::max(0, static_cast<int>(camera_topleft.x / bl_sz));
  int end_x   = static_cast<int>(Clamp(
    std::ceil((camera_topleft.x + screenWidth) / bl_sz), 0.0f,
    static_cast<float>(block_width_)));

  BeginMode2D(camera_);
  for (int i = start_x; i <= end_x; ++i)
    DrawLineV({i * bl_sz, 0.0f}, {i * bl_sz, screenHeight}, LIGHTGRAY);

  for (int i = 0; i <= screenHeight / blockSize; ++i)
    DrawLineV(
      {start_x * bl_sz, i * bl_sz}, {end_x * bl_sz, i * bl_sz}, LIGHTGRAY);
  EndMode2D();
}

void EditorScene::DrawCursor() {
  // Snapping grid
  Vector2 mouse_world_pos = GetScreenToWorld2D(GetMousePosition(), camera_);

  float bl_sz = static_cast<float>(blockSize);
  // Update only if mouse is in the boundary
  if (CheckCollisionPointRec(mouse_world_pos, boundary_)) {
    snapped_.x = std::floor(mouse_world_pos.x / bl_sz) * bl_sz;
    snapped_.y = std::floor(mouse_world_pos.y / bl_sz) * bl_sz;
  }

  Color transparent = {128, 128, 128, 128};

  BeginMode2D(camera_);
  DrawTexturePro(
    ground_tiles_, ground_tiles_info_[59],
    {snapped_.x, snapped_.y, bl_sz, bl_sz}, {0, 0}, 0, transparent);
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
    }
    fin.close();
  }
}
