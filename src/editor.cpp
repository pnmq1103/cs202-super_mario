#include "../tinyfiledialogs.h"
#include <cmath>
#include <fstream>
#include <iostream>
#include <raylib.h>
#include <raymath.h>

#include "include/application.hpp"
#include "include/editor.hpp"

EditorScene::EditorScene() {
  // The world position the camera is looking at (center of the view).
  camera.target = {0.0f, 0.0f};

  // The screen position where the target will appear.
  camera.offset   = {0.0f, 0.0f};
  camera.rotation = 0.0f;
  camera.zoom     = 1.0f;
}

EditorScene::~EditorScene() {}

void EditorScene::Init() {}

void EditorScene::Update() {
  if (IsKeyPressed(KEY_ESCAPE))
    Application::ChangeScene(nullptr);

  if (IsKeyDown(KEY_LEFT))
    player.x -= 5.0f;
  else if (IsKeyDown(KEY_RIGHT))
    player.x += 5.0f;

  if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
    Vector2 delta = GetMouseDelta();
    delta         = Vector2Scale(delta, -1.0f / camera.zoom);
    camera.target = Vector2Add(camera.target, delta);
  }

  if (IsKeyDown(KEY_O))
    LoadFile();
}

void EditorScene::Draw() {
  float bl_sz = static_cast<float>(blockSize);

  Vector2 camera_topleft = {camera.target.x - camera.offset.x, 0.0f};
  int start_x = std::max(0, static_cast<int>(camera_topleft.x / bl_sz));
  int end_x   = static_cast<int>(
    Clamp(std::ceil((camera_topleft.x + screenWidth) / bl_sz), 0.0f, 50.0f));

  // Snapping grid
  Vector2 mouse_world_pos = GetScreenToWorld2D(GetMousePosition(), camera);

  float snapped_x = std::floor(mouse_world_pos.x / bl_sz) * bl_sz;
  float snapped_y = std::floor(mouse_world_pos.y / bl_sz) * bl_sz;

  BeginMode2D(camera);
  for (int i = start_x; i <= end_x; ++i)
    DrawLineV({i * bl_sz, 0.0f}, {i * bl_sz, screenHeight}, LIGHTGRAY);

  for (int i = 0; i <= screenHeight / blockSize; ++i)
    DrawLineV({start_x * bl_sz, i * bl_sz}, {end_x * bl_sz, i * bl_sz}, GRAY);

  DrawRectangleRec(player, RED);

  DrawRectangleRec({snapped_x, snapped_y, bl_sz, bl_sz}, GRAY);
  EndMode2D();
  DrawTextEx(GetFontDefault(), TextFormat("[%i, %i]", GetMouseX(), GetMouseY()),
             Vector2Add(GetMousePosition(), {-44, -24}), 20, 2, BLACK);
}

void EditorScene::LoadFile() {
  const char *filter[] = {"*.txt"};
  const char *file_path
    = tinyfd_openFileDialog("Select map",
                            "", // (last used folder)
                            1, filter, "Text file (*.txt)", 0);

  if (file_path) {
    std::cout << "Trying to open " << file_path << '\n';
    std::ifstream fin(file_path);
    if (fin.is_open()) {
      int n; 
      fin >> n;
      for (int i = 0; i < n; ++i)
        std::cout << i << ' ';
    } else {
    }
    fin.close();
  }
}
