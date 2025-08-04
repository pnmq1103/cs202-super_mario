#include <raylib.h>
#include <raymath.h>

#include "include/core/application.hpp"
#include "include/core/constants.hpp"
#include "include/core/scene.hpp"
#include "include/core/tile_selector.hpp"
#include "include/core/utility.hpp"

TileSelectorScene::TileSelectorScene(int &select_gidx)
    : gidx_ref_(select_gidx) {
  camera_.target   = {0, 0};
  camera_.offset   = {0, 0};
  camera_.rotation = 0;
  camera_.zoom     = 1; // Important

  buttons_.reserve(3);
}

TileSelectorScene::~TileSelectorScene() {}

void TileSelectorScene::Init() {
  sprite_sheet_ = &App.Resource().GetTileset('m');
  utility::ReadSpriteInfo(
    "res/sprites/tilesets/tileset_minimal.txt", sprite_info_);
  float width  = static_cast<float>(sprite_sheet_->width);
  float height = static_cast<float>(sprite_sheet_->height);
  boundary_    = {0, 0, width * 4, height * 4};
}

void TileSelectorScene::Update() {
  if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_T)) {
    App.RemoveScene();
    return;
  }

  //if (IsKeyPressed(KEY_TAB)) {
  //  cur_sheet_ = (cur_sheet_ + 1) % sprite_sheets_.size();
  //  UpdateTextureParameters();
  //}

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
  DrawRectangleLinesEx(boundary_, 2, BLACK);
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
  Vector2 scroll_offset_ = Vector2Scale(GetMouseWheelMoveV(), -1 * 30);
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

//void TileSelectorScene::UpdateTextureParameters() {
//  cur_texture_ = &FindTexture(sprite_sheets_[cur_sheet_].texture_key);
//  float width  = static_cast<float>(cur_texture_->width);
//  float height = static_cast<float>(cur_texture_->height);
//  boundary_    = {0, 0, width * 4, height * 4};
//}

const Texture &TileSelectorScene::FindTexture(std::string texture_key) const {
  std::unordered_map<std::string, const Texture *> mp;
  mp["tileset_ground"]      = &App.Resource().GetTileset('g');
  mp["tileset_underground"] = &App.Resource().GetTileset('u');
  mp["tileset_minimal"]     = &App.Resource().GetTileset('m');
  mp["bowser"]              = &App.Resource().GetEnemy('b');
  mp["minions"]             = &App.Resource().GetEnemy('m');
  mp["enemies_icon"]        = &App.Resource().GetEnemy('i');

  for (const auto &[key, texture] : mp) {
    if (key == texture_key)
      return *mp[key];
  }
  throw std::out_of_range("texture not found");
}

void TileSelectorScene::ChooseTile() {
  if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    App.Media().PlaySound("beep");

  //  Because the display is scaled up 4 times
  mouse_world_pos_ = Vector2Scale(mouse_world_pos_, 0.25f);

  //const auto &sheet = sprite_sheets_[cur_sheet_];
  for (const auto &[local_idx, bounds] : sprite_info_) {
    if (CheckCollisionPointRec(mouse_world_pos_, bounds)) {
      gidx_ref_ = first_gidx_ + local_idx;
      break;
    }
  }
}
