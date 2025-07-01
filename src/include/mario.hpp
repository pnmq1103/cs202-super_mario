#pragma once
#include "include/character_state.hpp"

class NormalMario : public CharacterState {
private:
  Texture pre_texture_;
  Rectangle pre_frame_;

public:
  NormalMario(Character *Ncontext, float Nscale, bool Nleft, bool tran);
  ~NormalMario();

  void Die();
  void Update();
  void Evolve();
  void Draw();
};

class BigMario : public CharacterState {
private:
  Texture pre_texture_;
  Rectangle pre_frame_;

public:
  BigMario(Character *Ncontext, float Nscale, bool Nleft, bool tran);
  ~BigMario();

  void Update();
  void Die();
  void Evolve();
  void Draw();
};

class FireMario : public CharacterState {
private:
  Texture pre_texture_;
  Rectangle pre_frame_;

public:
  FireMario(Character *Ncontext, float Nscale, bool Nleft, bool tran);
  ~FireMario();

  void Die();
  void Evolve();
  void Update();
  void Draw();
};
