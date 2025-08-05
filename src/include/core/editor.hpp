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

  int select_gidx_ = {0};
  Map map_;
  MapLayer cur_layer_ = {MapLayer::Tile1};

  std::vector<Button> buttons_;
  bool button_clicked_ = {false};

private:
  void LoadMap();
  void SaveMap();
  void SaveMapAs();
  void CreateButtons();

  void PlaceBlock();
  void UpdateCamera();
  void UpdateMouse();
  void UpdateButtons();
  void UpdateShortkeys();

  void DrawGrid();
  void DrawMap();
  void DrawObjects();
  void DrawPipe();
  void DrawButtons();
  void DrawCursor();

public:
  EditorScene();
  ~EditorScene();

  void Init() override;
  void Update() override;
  void Draw() override;
  void Resume() override;
  SceneType Type() override;
};