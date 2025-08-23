#include <cmath>
#include <fstream>
#include <iostream>
#include <raylib.h>
#include <stdexcept>
#include <string>
#include <vector>

#include "include/characters/character_state.hpp"
#include "include/core/constants.hpp"

CharacterState::CharacterState(
  int NrunTime, float Nspeed, int NjumpTime, float NjumpHeight, float Nscale,
  bool Nto_left)
    : runTime(NrunTime), speed(Nspeed), jumpTime(NjumpTime),
      jumpHeight(NjumpHeight), scale(Nscale) {
  time_x         = -1;
  time           = 0;
  to_left        = Nto_left;
  is_jump        = false;
  is_fall        = false;
  is_die         = false;
  disabled       = false;
  stop_direction = 0;
  time_star      = 0;
  x_stop         = INT_MIN;
  velocity_x     = 0;
}
CharacterState::~CharacterState() {}

void CharacterState::LoadFrameList(std::string address) {
  std::ifstream fin;
  fin.open(address);
  if (!fin.is_open())
    throw std::runtime_error("cannot open file");
  while (!fin.eof()) {
    Rectangle rect;
    float n;
    fin >> n;
    fin >> n;
    rect.x = n;
    fin >> n;
    rect.y = n;
    fin >> n;
    rect.width = n;
    fin >> n;
    rect.height = n;
    frame_list.push_back(rect);
  }
  fin.close();
}

Rectangle CharacterState::MakeDestRect(Rectangle rectangle) {
  return {
    position.x, position.y, rectangle.width * scale, rectangle.height * scale};
}

Rectangle CharacterState::GetRectangle() {
  Rectangle rect = {
    position.x, position.y, frame_list[0].width * scale,
    frame_list[0].height * scale};
  return rect;
}

Vector2 CharacterState::GetSpeed() {
  Vector2 velocity;
  if (time_x == -1)
    velocity.x = 0;
  else {
    if (to_left)
      velocity.x = -speed;
    else
      velocity.x = speed;
  }
  velocity.y = velocity_y;
  return velocity;
}

bool CharacterState::IsToLeft() {
  if (frame.width > 0)
    return false;
  else
    return true;
}

void CharacterState::SetPosition(Vector2 Nposition) {
  position = Nposition;
}
void CharacterState::SetFrameCount() {
  ++time;
  if (time_star > 0)
    --time_star;
}

void CharacterState::Draw() {
  std::vector<Color> color_set
    = {RED, ORANGE, YELLOW, GREEN, BLUE, PURPLE, WHITE};
  if (time_star > 0) {
    DrawTexturePro(
      *texture, frame, MakeDestRect(frame), {0, 0}, 0.f,
      color_set[(time_star / 5) % 7]);
  } else
    DrawTexturePro(*texture, frame, MakeDestRect(frame), {0, 0}, 0.f, WHITE);
}
void CharacterState::Jump() {
  if (disabled)
    return;
  if (velocity_y == 0 || is_die) {
    y_before_jump = position.y;
    velocity_y    = -4.0 * jumpHeight / jumpTime;
    is_jump       = true;
    position.y   -= 10;
  }
}

void CharacterState::Run(bool to_left) {
  if (disabled)
    return;
  if (time_x == -1) {
    time_x        = time;
    this->to_left = to_left;
  }
}

void CharacterState::StopX() {
  if (is_die)
    return;
  int n;
  if (to_left)
    n = -1;
  else
    n = 1;

  if (stop_direction != 0 && stop_direction != n) {
    stop_direction = 0;
    x_stop         = INT_MIN;
    return;
  }
  if (stop_direction == 0)
    stop_direction = n;
  time_x     = -1;
  x_stop     = position.x;
  velocity_x = 0;
}
void CharacterState::StopY(float Ny) {
  if (is_die)
    return;
  if (is_jump) {
    is_jump = false;
    return;
  }
  is_fall    = false;
  velocity_y = -8.0 * jumpHeight / (jumpTime * jumpTime);
  position.y = Ny - frame_list[0].height * scale;
  frame      = frame_list[0];
  if (to_left) {
    frame.width = -fabs(frame.width);
  }
}
void CharacterState::StopY() {
  if (is_die)
    return;
  velocity_y = 0;
  is_fall    = true;
}
bool CharacterState::IsFalling() {
  return is_fall;
}

void CharacterState::Update() {

  if (stop_direction == 1) {
    if (position.x > x_stop) {
      stop_direction = 0;
      x_stop         = INT_MIN;
    }

  } else if (stop_direction == -1) {
    if (position.x < x_stop) {
      stop_direction = 0;
      x_stop         = INT_MIN;
    }
  }

  float a     = 8.0 * jumpHeight / (jumpTime * jumpTime);
  velocity_y += a;
  position.y += velocity_y;

  if (velocity_y > 0)
    is_fall = true;

  if (velocity_x < 0)
    velocity_x = 0;
  if (velocity_x > speed)
    velocity_x = speed;

  if (time_x != -1) {
    if (velocity_y == 0) {
      if (time - time_x == runTime) {
        time_x = -1;
      }

    } else {
      time_x = -1;
    }
    if (to_left)
      position.x -= speed;
    else
      position.x += speed;
  }

  if (position.x < 0)
    position.x = 0;
  if (
    position.x + frame.width * scale
    > constants::mapWidth * constants::blockSize)
    position.x
      = constants::mapWidth * constants::blockSize - frame.width * scale;
}
void CharacterState::ToStarman() {
  time_star = 60 * 10;
}
bool CharacterState::IsStarman() {
  if (time_star == 0)
    return false;
  else
    return true;
}
void CharacterState::Bounce() {
  if (is_die)
    return;
  if (IsFalling())
    velocity_y *= -1.1;
}

bool CharacterState::IsDead() {
  return is_die;
}