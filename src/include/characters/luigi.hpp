#pragma once
#include <raylib.h>

#include "include/characters/character.hpp"
#include "include/characters/character_state.hpp"

class NormalLuigi : public CharacterState {
private:
  Texture pre_texture_;
  Rectangle pre_frame_;

public:
  NormalLuigi(
    Character *Ncontext, float Nscale, bool Nis_left, bool is_evolving);
  ~NormalLuigi();

  void Die();
  void Update();
  void Evolve();
  void Draw();
};

class BigLuigi : public CharacterState {
private:
  Texture pre_texture_;
  Rectangle pre_frame_;

public:
  BigLuigi(Character *Ncontext, float Nscale, bool Nis_left, bool is_evolving);
  ~BigLuigi();

  void Update();
  void Die();
  void Evolve();
  void Draw();
};

class FireLuigi : public CharacterState {
private:
  Texture pre_texture_;
  Rectangle pre_frame_;

public:
  FireLuigi(Character *Ncontext, float Nscale, bool Nis_left, bool is_evolving);
  ~FireLuigi();

  void Die();
  void Evolve();
  void Update();
  void Draw();
};