#pragma once
#include <unordered_map>
#include <vector>

#include "include/core/button.hpp"
#include "include/core/scene.hpp"

class EnemySelectorScene : public Scene {
private:
  SceneType type_ = {EnemySelector};
  int &gidx_ref_;

  int first_gidx = 77;

  const Texture *sprite_sheet_ = {nullptr};
  std::unordered_map<int, Rectangle> sprite_info_;

  Rectangle boundary_ = {};

public:
  EnemySelectorScene(int &gidx);
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