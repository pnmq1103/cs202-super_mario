#pragma once
#include <raylib.h>

#include "include/characters/character.hpp"

class Command {
private:
  Character *mario_            = {nullptr};
  Character *luigi_            = {nullptr};
  Character *active_character_ = {nullptr};
  bool fireball_active_        = {false}; // Only allow one fireball at a time

public:
  Command(Character *mario = nullptr, Character *luigi = nullptr);
  ~Command();

  // Set Mario and Luigi pointers
  void SetMario(Character *mario);
  void SetLuigi(Character *luigi);

  // Set the active character to control
  void SetActiveCharacter(Character *character);
  Character *GetActiveCharacter() const;

  // Switch between Mario and Luigi
  void SwitchCharacter();

  // Main input handling method
  void HandleInput();

  // Individual command methods
  void MoveCharacter(bool left);
  void StopCharacter();
  void JumpCharacter();
  void ShootFireball();

  // Fireball state management
  void SetFireballActive(bool active);
  bool IsFireballActive() const;
};