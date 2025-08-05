#pragma once
#include <raylib.h>
#include <string>
#include <vector>

#include "include/core/button.hpp"
#include "include/core/scene.hpp"
#include "include/characters/character.hpp"

enum class SelectedCharacter {
  MARIO,
  LUIGI
};

class CharacterSelectionScene : public Scene {
private:
  SceneType type_ = {CharacterSelection};

  Texture background_ = {};
  
  // Character selection state (now instance variable)
  SelectedCharacter selectedCharacter_ = {SelectedCharacter::MARIO};
  

  double last_input_ = {0};
  const double cooldown_ = {0.3}; 
  float timer_ = {0};
  float duration_ = {0.5f};
  static SelectedCharacter global_selected_character_;

public:
  CharacterSelectionScene();
  ~CharacterSelectionScene();

  void Init() override;
  void Update() override;
  void Draw() override;
  void Resume() override;
  SceneType Type() override;

  // Character selection methods
  void SelectMario();
  void SelectLuigi();
  void StartGame();
  void DrawCharacterOptions();
  void DrawInstructions();
  
  
  static SelectedCharacter GetSelectedCharacter() { return global_selected_character_; }
};