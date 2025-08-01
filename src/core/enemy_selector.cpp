#include <raylib.h>
#include <raymath.h>

#include "include/core/application.hpp"
#include "include/core/constants.hpp"
#include "include/core/enemy_selector.hpp"
#include "include/core/scene.hpp"
#include "include/core/utility.hpp"

EnemySelectorScene::EnemySelectorScene(int &gidx) : gidx_ref_(gidx) {
  camera_.target   = {0, 0};
  camera_.offset   = {0, 0};
  camera_.rotation = 0;
  camera_.zoom     = 1; // Important
}

EnemySelectorScene::~EnemySelectorScene() {}

void EnemySelectorScene::Init() {
  sprite_sheet_ = &App.Resource().GetEnemy('i');
  utility::ReadSpriteInfo("res/sprites/enemies/enemies_icon.txt", sprite_info_);

  float width  = static_cast<float>(sprite_sheet_->width);
  float height = static_cast<float>(sprite_sheet_->height);
  boundary_    = {0, 0, width * 4, height * 4};
}

void EnemySelectorScene::Update() {
  if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_E)) {
    App.RemoveScene();
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
  DrawTexturePro(
    *sprite_sheet_, {0, 0, width, height}, boundary_, {0, 0}, 0, WHITE);
  DrawRectangleLinesEx(boundary_, 2, BLACK);
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
  Vector2 scroll_offset_ = Vector2Scale(GetMouseWheelMoveV(), -20);
  camera_.target         = Vector2Add(camera_.target, scroll_offset_);

  // Restrict camera
  float world_width  = boundary_.x + boundary_.width;
  float world_height = boundary_.y + boundary_.height;
  if (constants::screenWidth > world_width)
    camera_.target.x = (world_width - constants::screenWidth) / 2;
  else
    camera_.target.x
      = Clamp(camera_.target.x, 0, world_width - constants::screenWidth);
  if (constants::screenHeight > world_height)
    camera_.target.y = (world_height - constants::screenHeight) / 2;
  else
    camera_.target.y
      = Clamp(camera_.target.y, 0, world_height - constants::screenHeight);
}

void EnemySelectorScene::ChooseTile() {
  if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    App.Media().PlaySound("beep");

  mouse_world_pos_ = Vector2Scale(mouse_world_pos_, 0.25f);
  for (const auto &[local_idx, bounds] : sprite_info_) {
    if (CheckCollisionPointRec(mouse_world_pos_, bounds)) {
      gidx_ref_ = first_gidx + local_idx;
      break;
    }
  }
}
