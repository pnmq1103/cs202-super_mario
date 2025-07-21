#pragma once
#include <unordered_map>
#include <vector>

#include "include/core/button.hpp"
#include "include/core/scene.hpp"

class TileSelectorScene : public Scene {
private:
  SceneType type_ = {TileSelector};

  int &selected_tile_id_ref_;

  const Texture *tileset_ground_ = {nullptr};
  std::unordered_map<int, Rectangle> tileset_ground_info_;
  Rectangle boundary_ = {};

  int grid_rows_ = {0};
  int grid_cols_ = {0};

  float scroll_speed_ = {30};
  std::vector<std::vector<int>> sprites_grid_;

  std::vector<Button> buttons_;

public:
  TileSelectorScene(int &selected_tile_id);
  ~TileSelectorScene();

  void Init() override;
  void Update() override;
  void Draw() override;
  SceneType Type() override;

  void UpdateCamera();

  void BuildSpriteGrid();
  void ChooseTile();

  void CreateButtons();
  void UpdateButtons();
  void DrawButtons();
};