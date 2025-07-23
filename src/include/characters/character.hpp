#pragma once
#include "include/core/resource_manager.hpp"
#include <raylib.h>

enum CharacterType {
  MARIO,
  LUIGI,
};

class CharacterState;

class Character {
private:
  CharacterState *pState_ = {nullptr};
  CharacterType type_     = {};
  int state_              = {0}; // 0 for normal, 1 for big, 2 for fire/electric
  float scale_            = {4};

public:
  Character(float Nscale);
  ~Character();
  void SetCharacter(CharacterType type, Vector2 position);

  // set is_ivolving to true if the character is created or is evolving from
  // other lower state.
  // set it to false if character is dying.
  void SetState(int state, bool is_evolving);
  CharacterType GetCharacter();
  int GetState();

  void Draw();
  void Run(bool to_left);
  void Jump();
  void Update();
  void SetFrameCount();
  void StopX();
  void StopY(float Ny);
  void StopY();
  Vector2 GetSpeed();
  bool IsFalling();
  void Die();
  void Evolve();
  void ToStarman();
  bool IsStarman();
  void Bounce();
  Rectangle GetRectangle();
};