#pragma once

#include "include/characters/character.hpp"
#include "include/core/command.hpp"
#include "include/core/game_managing.hpp"
#include "include/core/scene.hpp"

class GameScene : public Scene {
private:
  SceneType type_ = {Game};

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
  void Resume() override;
  SceneType Type() override;
};
