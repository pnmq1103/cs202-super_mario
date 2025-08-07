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

  Texture background_    = {};
  double last_input_     = {0};
  const double cooldown_ = {0.1};

  bool hover_left     = {true};
  Rectangle left_rec  = {};
  Rectangle right_rec = {};
  Triangle left_tri   = {};
  Triangle right_tri  = {};

private:
  void CreateRegions();
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