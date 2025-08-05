#pragma once
#include <unordered_map>
#include <vector>

#include "include/core/button.hpp"
#include "include/core/scene.hpp"

class EnemySelectorScene : public Scene {
private:
  SceneType type_ = {EnemySelector};
  int &gidx_ref_;

  int first_gidx = 79;

  std::unordered_map<int, Rectangle> sprite_info_;
  const Texture *sprite_sheet_ = {nullptr};

  Rectangle boundary_ = {};

private:
  void UpdateCamera();
  void ChooseTile();

public:
  EnemySelectorScene(int &gidx);
  ~EnemySelectorScene();

  void Init() override;
  void Update() override;
  void Draw() override;
  void Resume() override;
  SceneType Type() override;
};