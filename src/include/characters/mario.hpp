#pragma once
#include <raylib.h>

#include "../include/characters/character.hpp"
#include "../include/characters/character_state.hpp"

class NormalMario : public CharacterState {
private:
  Texture pre_texture_ = {};
  Rectangle pre_frame_ = {};

public:
  NormalMario(
    Character *Ncontext, float Nscale, bool Nto_left, bool is_evolving);
  ~NormalMario();

  void Die();
  void Update();
  void Evolve();
  void Draw();
};

class BigMario : public CharacterState {
private:
  Texture pre_texture_ = {};
  Rectangle pre_frame_ = {};

public:
  BigMario(Character *Ncontext, float Nscale, bool Nto_left, bool is_evolving);
  ~BigMario();

  void Update();
  void Die();
  void Evolve();
  void Draw();
};

class FireMario : public CharacterState {
private:
  Texture pre_texture_ = {};
  Rectangle pre_frame_ = {};

public:
  FireMario(Character *Ncontext, float Nscale, bool Nto_left, bool is_evolving);
  ~FireMario();

  void Die();
  void Evolve();
  void Update();
  void Draw();
};
