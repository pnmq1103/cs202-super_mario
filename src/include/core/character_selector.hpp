#pragma once
#include <raylib.h>
#include <string>
#include <vector>

#include "include/characters/character.hpp"
#include "include/core/button.hpp"
#include "include/core/scene.hpp"

struct Triangle {
  Vector2 a;
  Vector2 b;
  Vector2 c;
};

class CharacterSelectorScene : public Scene {
private:
  SceneType type_ = {CharacterSelector};

  static CharacterType selected_character_;

  Texture background_ = {};
  Texture mario_      = {};
  Texture luigi_      = {};

  double last_input_     = {0};
  const double cooldown_ = {0.1};

  bool hover_left = {true};

private:
  void DisplayPower();
  void StartGame();

public:
  CharacterSelectorScene() = default;
  ~CharacterSelectorScene();

  void Init() override;
  void Update() override;
  void Draw() override;
  void Resume() override;
  SceneType Type() override;

  static CharacterType GetCharacterType();
};