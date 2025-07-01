#pragma once
#include <raylib.h>

#include "character.hpp"
#include "include/character_state.hpp"

class NormalLuigi : public CharacterState {
private:
  Texture pre_texture_;
  Rectangle pre_frame_;

public:
  NormalLuigi(Character *Ncontext, float Nscale, bool Nleft, bool tran);
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
  BigLuigi(Character *Ncontext, float Nscale, bool Nleft, bool tran);
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
  FireLuigi(Character *Ncontext, float Nscale, bool Nleft, bool tran);
  ~FireLuigi();

  void Die();
  void Evolve();
  void Update();
  void Draw();
};