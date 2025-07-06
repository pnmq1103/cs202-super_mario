#pragma once

#include <unordered_map>

#include "scene.hpp"

class TileSelectorScene : public Scene {
private:
  int &selected_tile_id_ref_;
  static constexpr float cellSize = {16}; // int needs typecast

  Texture ground_tiles_ = {};
  std::unordered_map<int, Rectangle> ground_tiles_info_;
  Rectangle boundary_ = {};

  int grid_rows_ = {0};
  int grid_cols_ = {0};

  float scroll_speed_ = {30};
  std::vector<std::vector<int>> sprites_grid_;

public:
  TileSelectorScene(int &selected_tile_id);
  ~TileSelectorScene();

  void Init() override;
  void Update() override;
  void Draw() override;

  void CameraUpdate();

  void BuildSpriteGrid();
  void ChooseTile();
};