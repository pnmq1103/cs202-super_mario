#include <raylib.h>

#include "include/characters/character.hpp"
#include "include/core/command.hpp"

Command::Command(Character *mario, Character *luigi)
    : mario_(mario), luigi_(luigi), fireball_active_(false) {
  active_character_ = mario_ ? mario_ : luigi_;
}

Command::~Command() {}

void Command::SetMario(Character *mario) {
  mario_ = mario;
  if (!active_character_)
    active_character_ = mario_;
}

void Command::SetLuigi(Character *luigi) {
  luigi_ = luigi;
  if (!active_character_)
    active_character_ = luigi_;
}

void Command::SetActiveCharacter(Character *character) {
  active_character_ = character;
}

Character *Command::GetActiveCharacter() const {
  return active_character_;
}

void Command::SwitchCharacter() {
  if (active_character_ == mario_ && luigi_) {
    active_character_ = luigi_;
  } else if (active_character_ == luigi_ && mario_) {
    active_character_ = mario_;
  }
}

void Command::HandleInput() {
  if (!active_character_)
    return;

  // --- Handle Character Switch ---
  if (IsKeyPressed(KEY_TAB)) {
    SwitchCharacter();
  }

  // --- Handle Movement ---
  bool is_moving = false;
  if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
    MoveCharacter(true); // Move left
    is_moving = true;
  }
  if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
    MoveCharacter(false); // Move right
    is_moving = true;
  }
  if (!is_moving) {
    StopCharacter();
  }
  if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W) || IsKeyPressed(KEY_SPACE)) {
    JumpCharacter();
  }

  // --- Handle Fireball/Ice Ball ---
  if (
    (IsKeyPressed(KEY_LEFT_CONTROL) || IsKeyPressed(KEY_RIGHT_CONTROL)
     || IsKeyPressed(KEY_LEFT_SHIFT) || IsKeyPressed(KEY_RIGHT_SHIFT))
    && active_character_->GetState() == 2 && !fireball_active_) {
    ShootFireball();
    fireball_active_ = true;
  }

  // --- Handle Mouse Clicks (for menu or in-game actions) ---
  if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
    // Example: Could be used for menu or in-game object interaction
    // You can expand this to call a callback or interact with objects
  }
}

void Command::MoveCharacter(bool left) {
  if (active_character_) {
    active_character_->Run(left);
  }
}

void Command::StopCharacter() {
  if (active_character_) {
    active_character_->StopX();
  }
}

void Command::JumpCharacter() {
  if (active_character_) {
    active_character_->Jump();
  }
}

void Command::ShootFireball() {
  if (active_character_) {
    // Only allow one fireball at a time (to be implemented in
    // character/projectile system) Example: active_character_->ShootFireball();
    // You need to coordinate with the projectile system for actual fireball
    // creation When the fireball is destroyed, call SetFireballActive(false)
    // from the projectile system
  }
}

void Command::SetFireballActive(bool active) {
  fireball_active_ = active;
}

bool Command::IsFireballActive() const {
  return fireball_active_;
}