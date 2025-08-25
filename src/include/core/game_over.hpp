#pragma once

#include "include/core/scene.hpp"
#include "include/core/application.hpp"
#include "include/core/constants.hpp"

class GameOverScene : public Scene {
private:
  SceneType type_ = SceneType::GameOver;
  int current_frame = 0, anim_frames = 0, next_frame_offset = 0, frame_counter = 0;
  Image game_over_image;
  Texture game_over_texture;

public:
  GameOverScene() = default;
  ~GameOverScene() override = default;
  void Init() override;
  void Update() override;
  void Draw() override;
  void Resume() override;
  SceneType Type() override;
};