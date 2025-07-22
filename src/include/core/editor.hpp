#pragma once
#include <raylib.h>
#include <unordered_map>
#include <vector>

#include "include/core/button.hpp"
#include "include/core/scene.hpp"

class EditorScene : public Scene {
private:
  SceneType type_ = {Editor};

  int block_width_  = {50};
  int block_height_ = {50};

  Vector2 snapped_    = {0, 0};
  Rectangle boundary_ = {};

  Texture crosshair_ = {};
  std::vector<int> tilemap_;

  const Texture *tileset_ = {nullptr};
  std::unordered_map<int, Rectangle> tileset_info_;
  const Texture *background_ = {nullptr};
  std::unordered_map<int, Rectangle> background_info_;

  int global_selected_idx_ = {0};
  struct SpriteSheet {
    int first_gidx_         = {0};
    int count_              = {0};
    const Texture *texture_ = {nullptr};

    SpriteSheet(int first_gidx, int count, const Texture &texture)
        : first_gidx_(first_gidx), count_(count), texture_(&texture) {}
  };
  std::vector<SpriteSheet> sprite_sheets_;

  std::vector<Button> buttons_;
  bool button_clicked_ = {false};

  Vector2 drag_delta_ = {0, 0};

public:
  EditorScene();
  ~EditorScene();

  void Init() override;
  void Update() override;
  void Draw() override;
  SceneType Type() override;
  void Resume() override;

  void PlaceBlock();

  void DrawGrid();
  void DrawCursor();
  void DrawMap();

  void CreateButtons();
  void UpdateButtons();
  void DrawButtons();

  int FindLocalIndex(int gidx);
};

// Since the original game's resolution is 256x240 pixels
// and each block is 16x16 pixels.
// The default map should be 16x15 blocks.

// background layer:
// -gidx: 1
// - element: 10(0->9)

// tilset layer:
// ground
// - gidx: 11
// - element: 144

// ... other tilesets

// objects layer
// - gidx: ..
// - element: