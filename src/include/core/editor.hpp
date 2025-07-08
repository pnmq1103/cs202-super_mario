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

  Texture ground_tiles_ = {};
  std::unordered_map<int, Rectangle> ground_tiles_info_;

  int selected_tile_id_       = {0};
  int selected_enemy_id_      = {0};
  int selected_background_id_ = {0};

  std::vector<Button> buttons_;

public:
  EditorScene();
  ~EditorScene();

  void Init() override;
  void Update() override;
  void Draw() override;
  SceneType Type() override;

  // Update
  void PlaceBlock();

  // Draw
  void DrawGrid();
  void DrawCursor();
  void DrawMap();

  // Button
  void CreateButtons();
  void UpdateButtons();
  void DrawButtons();

  void LoadFile();
};

// Since the original game's resolution is 256x240 pixels
// and each block is 16x16 pixels.
// The default map should be 16x15 blocks.