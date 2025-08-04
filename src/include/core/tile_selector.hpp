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

  /*int cur_sheet_              = {0};
  const Texture *cur_texture_ = {nullptr};*/
  const Texture* sprite_sheet_;
  std::unordered_map<int, Rectangle> sprite_info_;
  int first_gidx_ = 1;

  Rectangle boundary_ = {};
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
  void UpdateTextureParameters();

  const Texture &FindTexture(std::string texture_key) const;
  void ChooseTile();

  void CreateButtons();
  void UpdateButtons();
  void DrawButtons();
};
