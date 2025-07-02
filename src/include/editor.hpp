#pragma once
#include "scene.hpp"
#include <raylib.h>
#include <unordered_map>

class EditorScene : public Scene {
private:
  static constexpr int blockSize = {16 * 4};

  int block_width_  = {50};
  int block_height_ = {15};

  Texture ground_tiles_ = {};
  std::unordered_map<int, Rectangle> ground_tiles_info_;

  Vector2 snapped_ = {0.0f, 0.0f};

public:
  EditorScene();
  ~EditorScene();

  void Init() override;
  void Update() override;
  void Draw() override;

  void DrawGrid();
  void DrawCursor();
  void LoadFile();
};

// Since the original game's resolution is 256x240 pixels
// and each block is 16x16 pixels.
// The default map should be 16x15 blocks.