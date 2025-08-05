#pragma once
#include <unordered_map>
#include <vector>

#include "include/core/button.hpp"
#include "include/core/map.hpp"
#include "include/core/scene.hpp"

class TileSelectorScene : public Scene {
private:
  SceneType type_ = {TileSelector};
  int &gidx_ref_;

  int first_gidx_ = 1;
  std::unordered_map<int, Rectangle> sprite_info_;
  const Texture *sprite_sheet_ = {nullptr};

  Rectangle boundary_ = {};
  std::vector<Button> buttons_;

private:
  void UpdateCamera();
  void ChooseTile();

public:
  TileSelectorScene(int &select_gidx);
  ~TileSelectorScene();

  void Init() override;
  void Update() override;
  void Draw() override;
  void Resume() override;
  SceneType Type() override;
};
