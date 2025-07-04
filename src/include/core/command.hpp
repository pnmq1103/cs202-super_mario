#pragma once
#include "../characters/character.hpp"
#include <raylib.h>

class Command {
private:
  Character *active_character_;

  // Input timing for smooth controls
  double last_input_time_;
  const double input_cooldown_  = 0.05; // Small cooldown for movement
  const double action_cooldown_ = 0.2;  // Longer cooldown for actions

public:
  Command(Character *character = nullptr);
  ~Command();

  // Set the active character to control
  void SetActiveCharacter(Character *character);

  // Main input handling method
  void HandleInput();

  // Individual command methods
  void MoveCharacter(bool left);
  void StopCharacter();
  void JumpCharacter();
  void ShootFireball();

  // Utility methods
  bool CanProcessInput() const;
  bool CanProcessAction() const;
  void UpdateInputTiming();
};