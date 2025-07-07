#pragma once

#include "../characters/character.hpp"
#include "command.hpp"
#include "game_managing.hpp"
#include "scene.hpp"

class GameScene : public Scene {
private:
  Character *mario_character_ = {nullptr};
  Character *luigi_character_ = {nullptr};
  Command *input_command_     = {nullptr};
  GameManaging game_manager_;

public:
  GameScene();
  ~GameScene();

  void Init() override;
  void Update() override;
  void Draw() override;
};
