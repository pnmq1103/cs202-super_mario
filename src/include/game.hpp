#pragma once

#include "character.hpp"
#include "command.hpp"
#include "scene.hpp"

class GameScene : public Scene {
private:
  Character *player_character_ = {nullptr};
  Command *input_command_      = {nullptr};

public:
  GameScene();
  ~GameScene();

  void Init() override;
  void Update() override;
  void Draw() override;
};
