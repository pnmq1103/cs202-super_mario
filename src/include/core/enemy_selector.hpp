#pragma once
#include <unordered_map>
#include <vector>

#include "include/core/button.hpp"
#include "include/core/scene.hpp"

class EnemySelectorScene : public Scene {
private:
  SceneType type_ = {EnemySelector};

  int &g_select_idx_ref_;

  int first_gidx = 299;

  const Texture *sprite_sheet_ = {nullptr};
  // since the enemies do not equal in sizes
  std::unordered_map<int, Rectangle> sprite_sheet_info_;
  Rectangle boundary_ = {};

  int grid_rows_ = {0};
  int grid_cols_ = {0};

  float scroll_speed_ = {20};

public:
  EnemySelectorScene(int &g_select_idx);
  ~EnemySelectorScene();

  void Init() override;
  void Update() override;
  void Draw() override;
  void Resume() override;
  SceneType Type() override;

private:
  void UpdateCamera();

  void ChooseTile();
};