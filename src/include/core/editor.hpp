#pragma once
#include <raylib.h>
#include <vector>

#include "include/core/button.hpp"
#include "include/core/map.hpp"
#include "include/core/scene.hpp"

class EditorScene : public Scene {
private:
  SceneType type_ = {Editor};

  int block_width_  = {50};
  int block_height_ = {50};

  Vector2 snapped_    = {0, 0};
  Rectangle boundary_ = {};

  Texture crosshair_ = {};

  int select_gidx_ = {0};
  Map map_;

  std::vector<Button> buttons_;
  bool button_clicked_ = {false};

  Vector2 drag_delta_ = {0, 0};

public:
  EditorScene();
  ~EditorScene();

  void Init() override;
  void Update() override;
  void Draw() override;
  void Resume() override;
  SceneType Type() override;

  void PlaceBlock();

  void DrawGrid();
  void DrawCursor();
  void DrawMap();

  void CreateButtons();
  void UpdateButtons();
  void DrawButtons();
};

// Since the original game's resolution is 256x240 pixels
// and each block is 16x16 pixels.
// The default map should be 16x15 blocks.