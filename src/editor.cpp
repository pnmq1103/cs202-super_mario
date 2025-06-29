#include <cmath>
#include <raylib.h>
#include <raymath.h>

#include "include/application.hpp"
#include "include/editor.hpp"

EditorScene::EditorScene() {
  camera.target   = {0.0f, 0.0f};
  camera.offset   = {screenWidth / 2.0f, screenHeight / 2.0f};
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
  if (IsKeyDown(KEY_RIGHT))
    player.x += 5.0f;
}

void EditorScene::Draw() {
  camera.target = {player.x, player.y};

  float bl_sz = static_cast<float>(blockSize);

  Vector2 camera_topleft = {camera.target.x - camera.offset.x, 0.0f};
  int start_x = std::max(0, static_cast<int>(camera_topleft.x / bl_sz));
  int end_x   = static_cast<int>(
    Clamp(std::ceil((camera_topleft.x + screenWidth) / bl_sz), 0.0f, 50.0f));

  BeginMode2D(camera);
  for (int i = start_x; i <= end_x; ++i)
    DrawLineV({i * bl_sz, 0.0f}, {i * bl_sz, screenHeight}, GRAY);

  for (int i = 0; i <= screenHeight / blockSize; ++i)
    DrawLineV({start_x * bl_sz, i * bl_sz}, {end_x * bl_sz, i * bl_sz}, GRAY);

  DrawRectangleRec(player, RED);
  EndMode2D();
}
