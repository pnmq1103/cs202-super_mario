#pragma once
#include <unordered_map>
#include <vector>

#include "include/core/button.hpp"
#include "include/core/scene.hpp"

class TileSelectorScene : public Scene {
private:
  SceneType type_ = {TileSelector};

  int &select_gidx_ref_;

  int first_gidx = 1; // need to support more first gidx

  const Texture *sprite_sheet_ = {nullptr};
  Rectangle boundary_          = {};

  int grid_rows_ = {0};
  int grid_cols_ = {0};

  float scroll_speed_ = {30};

  std::vector<Button> buttons_;

public:
  TileSelectorScene(int &g_select_idx);
  ~TileSelectorScene();

  void Init() override;
  void Update() override;
  void Draw() override;
  void Resume() override;
  SceneType Type() override;

private:
  void UpdateCamera();
  void ChooseTile();

  void CreateButtons();
  void UpdateButtons();
  void DrawButtons();
};
