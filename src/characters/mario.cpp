#include <cstdlib>
#include <raylib.h>

#include "include/characters/character.hpp"
#include "include/characters/character_state.hpp"
#include "include/characters/mario.hpp"

//+----------------------------------------------------------+
//|                       Normal Mario                       |
//+----------------------------------------------------------+

NormalMario::NormalMario(
  Character *Ncontext, float Nscale, bool Nto_left, bool is_evolving)
    : CharacterState(16, 10, 50, 300, Nscale, Nto_left) {
  if (!is_evolving) {
    disabled    = true;
    Image image = LoadImage("res/sprites/characters/mario_star.png");
    ImageResize(&image, image.width * scale, image.height * scale);
    pre_texture_ = LoadTextureFromImage(image);
    UnloadImage(image);
    pre_frame_ = {254 * scale, 0, 14 * scale, 27 * scale};
  }
  Image image = LoadImage("res/sprites/characters/mario_normal.png");
  ImageResize(&image, image.width * scale, image.height * scale);
  texture = LoadTextureFromImage(image);
  UnloadImage(image);
  context = Ncontext;
  LoadFrameList("res/sprites/characters/mario_normal.txt");
  frame = frame_list[0];
};

NormalMario::~NormalMario() {
  UnloadTexture(pre_texture_);
}

void NormalMario::Die() {
  if (disabled)
    return;
  CharacterState::Jump();
  is_die = true;
}

void NormalMario::Evolve() {
  if (disabled)
    return;
  context->SetState(1, true);
}

void NormalMario::Update() {
  if (disabled)
    return;
  CharacterState::Update();
  if (velocity_y != 0) {
    frame = frame_list[8];
  }

  if (time_x != -1 && time - time_x != runTime && velocity_y == 0) {
    frame = frame_list[(time - time_x) / (runTime / 2) + 3];
  }

  if (to_left) {
    frame.width = -abs(frame.width);
  } else
    frame.width = abs(frame.width);
}

void NormalMario::Draw() {
  if (disabled) {
    if (time / 5 % 2 == 0) {
      if (to_left) {
        frame.width = -abs(frame.width);
      } else
        frame.width = abs(frame.width);
      DrawTextureRec(texture, frame, position, WHITE);
    } else {
      if (to_left) {
        pre_frame_.width = -abs(pre_frame_.width);
      } else
        pre_frame_.width = abs(pre_frame_.width);
      DrawTextureRec(pre_texture_, pre_frame_, position, WHITE);
    }
    if (time == 30) {
      disabled = false;
      UnloadTexture(pre_texture_);
    }
  } else {
    CharacterState::Draw();
  }
}

// +----------------------------------------------------------+
// |                        Big Mario                         |
// +----------------------------------------------------------+

BigMario::BigMario(
  Character *Ncontext, float Nscale, bool Nleft, bool is_evolving)
    : CharacterState(15, 10, 50, 300, Nscale, Nleft) {
  disabled    = true;
  Image image = LoadImage("res/sprites/characters/mario_star.png");
  ImageResize(&image, image.width * scale, image.height * scale);
  texture = LoadTextureFromImage(image);
  UnloadImage(image);
  context = Ncontext;
  LoadFrameList("res/sprites/characters/mario_star.txt");
  frame = frame_list[0];

  if (is_evolving) {
    image      = LoadImage("res/sprites/characters/mario_normal.png");
    pre_frame_ = {0, 0, 12 * scale, 15 * scale};
  } else {
    image      = LoadImage("res/sprites/characters/mario_fire.png");
    pre_frame_ = frame_list[0];
  }
  ImageResize(&image, image.width * scale, image.height * scale);
  pre_texture_ = LoadTextureFromImage(image);
  UnloadImage(image);
}

BigMario::~BigMario() {
  UnloadTexture(pre_texture_);
}

void BigMario::Die() {
  if (disabled)
    return;
  context->SetState(0, false);
}

void BigMario::Evolve() {
  if (disabled)
    return;
  context->SetState(2, true);
}

void BigMario::Update() {
  if (disabled)
    return;
  CharacterState::Update();
  float v0 = 4.0f * jumpHeight / jumpTime;
  if (velocity_y != 0) {
    if (velocity_y < -v0 * 1 / 3) {
      frame = frame_list[6];
    } else if (velocity_y > v0 * 1 / 3) {
      frame = frame_list[8];
    } else {
      frame = frame_list[7];
    }
  }

  if (time_x != -1 && time - time_x != runTime && velocity_y == 0) {
    frame = frame_list[(time - time_x) / (runTime / 3) + 3];
  }

  if (to_left) {
    frame.width = -abs(frame.width);
  } else
    frame.width = abs(frame.width);
}

void BigMario::Draw() {
  if (time <= 30) {
    if (time / 5 % 2 == 0) {
      if (to_left) {
        frame.width = -abs(frame.width);
      } else
        frame.width = abs(frame.width);
      DrawTextureRec(texture, frame, position, WHITE);
    } else {
      if (to_left) {
        pre_frame_.width = -abs(pre_frame_.width);
      } else
        pre_frame_.width = abs(pre_frame_.width);
      DrawTextureRec(pre_texture_, pre_frame_, position, WHITE);
    }
    if (time == 30)
      UnloadTexture(pre_texture_);
  } else {
    disabled = false;
    CharacterState::Draw();
  }
}

//+----------------------------------------------------------+
//|                        Fire Mario                        |
//+----------------------------------------------------------+

FireMario::FireMario(
  Character *Ncontext, float Nscale, bool Nleft, bool is_evolving)
    : CharacterState(15, 10, 50, 300, Nscale, Nleft) {
  LoadFrameList("res/sprites/characters/mario_star.txt");
  if (is_evolving) {
    disabled    = true;
    Image image = LoadImage("res/sprites/characters/mario_star.png");
    ImageResize(&image, image.width * scale, image.height * scale);
    pre_texture_ = LoadTextureFromImage(image);
    UnloadImage(image);
    pre_frame_ = frame_list[0];
  }
  Image image = LoadImage("res/sprites/characters/mario_fire.png");
  ImageResize(&image, image.width * scale, image.height * scale);
  texture = LoadTextureFromImage(image);
  UnloadImage(image);
  context = Ncontext;
  frame   = frame_list[0];
};

FireMario::~FireMario() {
  UnloadTexture(pre_texture_);
}

void FireMario::Die() {
  if (disabled)
    return;
  context->SetState(1, false);
}
void FireMario::Evolve() {}

void FireMario::Update() {
  if (disabled)
    return;
  CharacterState::Update();
  float v0 = 4.0f * jumpHeight / jumpTime;
  if (velocity_y != 0) {
    if (velocity_y < -v0 * 1 / 3) {
      frame = frame_list[6];
    } else if (velocity_y > v0 * 1 / 3) {
      frame = frame_list[8];
    } else {
      frame = frame_list[7];
    }
  }

  if (time_x != -1 && time - time_x != runTime && velocity_y == 0) {
    frame = frame_list[(time - time_x) / (runTime / 3) + 3];
  }

  if (to_left) {
    frame.width = -abs(frame.width);
  } else
    frame.width = abs(frame.width);
}

void FireMario::Draw() {
  if (disabled) {
    if (time / 5 % 2 == 0) {
      if (to_left) {
        frame.width = -abs(frame.width);
      } else
        frame.width = abs(frame.width);
      DrawTextureRec(texture, frame, position, WHITE);
    } else {
      if (to_left) {
        pre_frame_.width = -abs(pre_frame_.width);
      } else
        pre_frame_.width = abs(pre_frame_.width);
      DrawTextureRec(pre_texture_, pre_frame_, position, WHITE);
    }
    if (time == 30) {
      disabled = false;
      UnloadTexture(pre_texture_);
    }
  } else {
    CharacterState::Draw();
  }
}
