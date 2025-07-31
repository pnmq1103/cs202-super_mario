#pragma once
#include <raylib.h>
#include <vector>

#include "include/core/button.hpp"
#include "include/core/map.hpp"
#include "include/core/scene.hpp"

class EditorScene : public Scene {
private:
  SceneType type_ = {Editor};

  Vector2 snap_       = {0, 0};
  Rectangle boundary_ = {};
  Vector2 drag_delta_ = {0, 0};

  int select_gidx_ = {0};
  Map map_;
  MapLayer cur_layer_ = MapLayer::Tile1;

  std::vector<Button> buttons_;
  bool button_clicked_ = {false};

public:
  EditorScene();
  ~EditorScene();

  void Init() override;
  void Update() override;
  void Draw() override;
  void Resume() override;
  SceneType Type() override;

  void CreateButtons();

  void UpdateCamera();
  void UpdateMouse();
  void UpdateButtons();
  void UpdateShortkeys();

  void PlaceBlock();

  void DrawGrid();
  void DrawMap();
  void DrawObjects();
  void DrawPipe();
  void DrawButtons();
  void DrawCursor();
};

// Since the original game's resolution is 256x240 pixels
// and each block is 16x16 pixels.
// The default map should be 16x15 blocks.