#pragma once
#include <raylib.h>
#include <string>
#include <vector>

#include "character.hpp"

// Time will be counted by frame not by second

class CharacterState {
protected:
  float y_before_jump, velocity_y, x_stop;
  int time_x, time, stop_direction, time_star;
  bool to_left, is_jump, is_fall, is_die, disabled;

  Character *context;
  Vector2 position;
  Texture texture;
  const float speed, jumpHeight, scale;
  const int jumpTime, runTime;
  Rectangle frame;

  std::vector<Rectangle> frame_list;
  void LoadFrameList(std::string address);
  void SetPosition(Vector2 Nposition);

public:
  CharacterState(
    int NrunTime, float Nspeed, int NjumpTime, float NjumpHeight, float Nscale,
    bool Nto_left);
  virtual ~CharacterState();

  Rectangle GetRectangle();

  void SetFrameCount();
  float GetSpeed();

  virtual void Draw();
  void Run(bool to_left);
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