#pragma once
#include <raylib.h>

enum CharacterType { MARIO, LUIGI };

class CharacterState;
class Character {
private:
  CharacterState *pState_;
  CharacterType type_;
  int state_;
  float scale_;

public:
  Character(float Nscale);
  ~Character();
  void SetCharacter(CharacterType type, Vector2 pos);
  void SetState(int state, bool tran);
  CharacterType GetCharacter();
  int GetState();

  void Draw();
  void Run(bool left);
  void Jump();
  void Update();
  void SetFrameCount();
  void StopX();
  void StopY(float Ny);
  void StopY();
  float GetSpeed();
  bool IsFalling();
  void Die();
  void Evolve();
  void ToStarman();
  bool IsStarman();
  Rectangle GetRectangle();
};