#pragma once
#include <raylib.h>
#include <unordered_map>

#include "scene.hpp"

class EditorScene : public Scene {
private:
  static constexpr float blockSize = {16 * 4}; // int needs typecast

  int block_width_  = {50};
  int block_height_ = {15};

  Vector2 mouse_world_pos_ = {0, 0};
  Vector2 snapped_         = {0, 0};
  Rectangle boundary_      = {};

  Texture crosshair_    = {};
  Texture ground_tiles_ = {};
  std::unordered_map<int, Rectangle> ground_tiles_info_;
  std::vector<int> tilemap_;

public:
  EditorScene();
  ~EditorScene();

  void Init() override;
  void Update() override;
  void Draw() override;

  void PlaceBlock();

  void DrawGrid();
  void DrawCursor();
  void DrawMap();

  void LoadFile();
};

// Since the original game's resolution is 256x240 pixels
// and each block is 16x16 pixels.
// The default map should be 16x15 blocks.