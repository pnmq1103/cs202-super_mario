#pragma once

#include "character.hpp"
#include "command.hpp"
#include "scene.hpp"
#include "include/gamemanaging.hpp"

class GameScene : public Scene {
private:
  Character *player_character_ = {nullptr};
  Command *input_command_      = {nullptr};
  GameManaging game_manager_; // Add GameManaging instance

public:
  GameScene();
  ~GameScene();

  void Init() override;
  void Update() override;
  void Draw() override;
};
