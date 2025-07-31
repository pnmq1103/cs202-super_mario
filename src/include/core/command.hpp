#pragma once
#include <raylib.h>

#include "include/characters/character.hpp"
#include "include/core/button.hpp"

// Forward declaration
class ProjectilePool;

class Command {
private:
  Character *mario_            = {nullptr};
  Character *luigi_            = {nullptr};
  Character *active_character_ = {nullptr};
  bool fireball_active_        = {false}; // Only allow one fireball at a time
  ProjectilePool *projectile_pool_ = {nullptr}; // Reference to projectile pool
  
  // Instructions system
  bool show_instructions_      = {false};
  Button instructions_button_;
  bool instructions_initialized_ = {false};

public:
  Command(Character *mario = nullptr, Character *luigi = nullptr);
  ~Command();

  // Set Mario and Luigi pointers
  void SetMario(Character *mario);
  void SetLuigi(Character *luigi);

  // Set the projectile pool reference
  void SetProjectilePool(ProjectilePool *projectile_pool);

  // Set the active character to control
  void SetActiveCharacter(Character *character);
  Character *GetActiveCharacter() const;

  // Switch between Mario and Luigi
  void SwitchCharacter();

  // Main input handling method
  void HandleInput();

  // Individual command methods - ARROW KEYS ONLY
  void MoveCharacter(bool left);
  void StopCharacter();
  void JumpCharacter(); // Added back for UP arrow key
  void ShootFireball();

  // Fireball state management
  void SetFireballActive(bool active);
  bool IsFireballActive() const;
  
  // Instructions system
  void InitializeInstructionsButton();
  void UpdateInstructionsButton();
  void DrawInstructionsButton();
  void DrawInstructionsPanel();
  bool IsShowingInstructions() const;
  void ToggleInstructions();
};