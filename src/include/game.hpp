#pragma once

#include "scene.hpp"
#include "character.hpp"
#include "command.hpp"

class GameScene : public Scene {
private:
  Character* player_character_;
  Command* input_command_;
  
public:
  GameScene();
  ~GameScene();

  void Init() override;
  void Update() override;
  void Draw() override;
};
