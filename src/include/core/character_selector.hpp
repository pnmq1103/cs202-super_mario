#pragma once
#include <raylib.h>
#include <string>
#include <vector>

#include "include/characters/character.hpp"
#include "include/core/button.hpp"
#include "include/core/scene.hpp"

enum class SelectedCharacter {
  MARIO,
  LUIGI,
};

class CharacterSelectorScene : public Scene {
private:
  SceneType type_ = {CharacterSelector};

  // Character selection state (now instance variable)
  SelectedCharacter selectedCharacter_ = {SelectedCharacter::MARIO};
  static SelectedCharacter global_selected_character_;

  Texture background_    = {};
  double last_input_     = {0};
  const double cooldown_ = {0.3};

private:
  void SelectMario();
  void SelectLuigi();
  void StartGame();

  void DrawCharacterOptions();
  void DrawInstructions();

public:
  CharacterSelectorScene();
  ~CharacterSelectorScene();

  void Init() override;
  void Update() override;
  void Draw() override;
  void Resume() override;
  SceneType Type() override;

  static SelectedCharacter GetSelectedCharacter() {
    return global_selected_character_;
  }
};