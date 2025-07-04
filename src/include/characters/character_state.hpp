#pragma once
#include <raylib.h>
#include <string>
#include <vector>

#include "character.hpp"

class CharacterState {
protected:
  float y, v, stop_x;
  int t_x, t, stop_left, t_star;
  bool left, is_jump, is_fall, is_die, disabled;

  Character *context;
  Vector2 pos;
  Texture2D texture;
  const float speed, jH, scale;
  const int jT, rT;
  Rectangle frame;

  std::vector<Rectangle> frame_list;
  void LoadFrameList(std::string at);
  void SetPosition(Vector2 Npos);

public:
  CharacterState(
    int NrT, float Nspeed, int NjT, float NjH, float Nscale, bool Nleft);
  virtual ~CharacterState();

  Rectangle GetRectangle();

  void SetFrameCount();
  float GetSpeed();

  virtual void Draw();
  void Run(bool left);
  void Jump();
  virtual void Update();
  void StopX();
  void StopY(float Ny);
  void StopY();
  bool IsFalling();
  virtual void Die()    = 0;
  virtual void Evolve() = 0;
  void ToStarman();
  bool IsStarman();

  friend class Character;
};