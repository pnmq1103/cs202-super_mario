#pragma once
#include <raylib.h>
#include <vector>

enum CharacterType { MARIO, LUIGI };
class CharacterState;
class Character {
private:
  CharacterState *pState;
  CharacterType type;
  int state;
  float scale;

public:
  Character(float Nscale);
  ~Character();
  void setCharacter(CharacterType type, Vector2 pos);
  void setState(int state);
  void draw();
  void run(bool left);
  void jump();
  void update();
  void setFrameCount();
};
