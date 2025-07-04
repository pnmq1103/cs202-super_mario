#include <raylib.h>

#include "../include/characters/character.hpp"
#include "../include/core/command.hpp"

Command::Command(Character *character)
    : active_character_(character), last_input_time_(0.0) {}

Command::~Command() {
  // No cleanup needed as we don't own the character
}

void Command::SetActiveCharacter(Character *character) {
  active_character_ = character;
}

void Command::HandleInput() {
  if (!active_character_) {
    return;
  }

  UpdateInputTiming();

  // Handle movement input
  bool is_moving = false;

  if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
    if (CanProcessInput()) {
      MoveCharacter(true); // Move left
      is_moving = true;
    }
  }

  if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
    if (CanProcessInput()) {
      MoveCharacter(false); // Move right
      is_moving = true;
    }
  }

  // Stop character if no movement keys are pressed
  if (!is_moving) {
    StopCharacter();
  }

  // Handle jump input
  if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) {
    if (CanProcessAction()) {
      JumpCharacter();
    }
  }

  // Handle fireball shooting (only for Fire Mario/Luigi)
  if (IsKeyPressed(KEY_X) || IsKeyPressed(KEY_LEFT_CONTROL)) {
    if (CanProcessAction()) {
      ShootFireball();
    }
  }
}

void Command::MoveCharacter(bool left) {
  if (!active_character_) {
    return;
  }

  active_character_->Run(left);
}

void Command::StopCharacter() {
  if (!active_character_) {
    return;
  }

  active_character_->StopX();
}

void Command::JumpCharacter() {
  if (!active_character_) {
    return;
  }

  active_character_->Jump();
  last_input_time_ = GetTime(); // Update timing for action cooldown
}

void Command::ShootFireball() {
  if (!active_character_) {
    return;
  }

  // Check if character is in Fire state (state 2)
  if (active_character_->GetState() == 2) {
    // Note: Shooting fireball functionality would need to be implemented
    // in the character system. For now, we just mark the action.
    // You might want to add a ShootFireball method to Character class
    last_input_time_ = GetTime(); // Update timing for action cooldown

    // Placeholder - this would trigger fireball creation
    // active_character_->ShootFireball();
  }
}

bool Command::CanProcessInput() const {
  double current_time = GetTime();
  return (current_time - last_input_time_) >= input_cooldown_;
}

bool Command::CanProcessAction() const {
  double current_time = GetTime();
  return (current_time - last_input_time_) >= action_cooldown_;
}

void Command::UpdateInputTiming() {
  // This gets called every frame to update timing for smooth movement
  double current_time = GetTime();
  if ((current_time - last_input_time_) >= input_cooldown_) {
    last_input_time_ = current_time;
  }
}