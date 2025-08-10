#pragma once

#include "include/characters/character.hpp"
#include "include/collision/collision_handler.hpp"
#include "include/core/command.hpp"
#include "include/core/game_managing.hpp"
#include "include/core/scene.hpp"

class GameScene : public Scene {
private:
  SceneType type_ = {Game};

  CharacterType character_type_;
  Character *player_character_
    = {nullptr}; // Single character pointer instead of mario/luigi
  Command *input_command_ = {nullptr};
  GameManaging game_manager_;
  static CollisionHandler collision_handler_;
  int current_level_ = 1;
  std::string levelPath = {"res/maps/map1.json"};
  bool is_level_loaded_ = false;

public:
  GameScene(CharacterType type, int level = 1);
  ~GameScene();
  // Add to GameScene class:
  GameManaging &GetGameManager() {
    return game_manager_;
  }

  void Init() override;
  void Update() override;
  void Draw() override;
  void Resume() override;
  SceneType Type() override;
  //void CreateSimpleTestLevel();
  void SetLevelPath(const std::string &path);

private:
  void UpdateCamera(Character *character);
};
