#include <cstdlib>
#include <raylib.h>

#include "include/character.hpp"
#include "include/character_state.hpp"
#include "include/mario.hpp"

//+----------------------------------------------------------+
//|                       Normal Mario                       |
//+----------------------------------------------------------+

NormalMario::NormalMario(
  Character *Ncontext, float Nscale, bool Nleft, bool tran)
    : CharacterState(16, 10, 50, 300, Nscale, Nleft) {
  if (!tran) {
    disabled    = true;
    Image image = LoadImage("res/sprites/characters/mario_starup.png");
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
  LoadFrameList("res/sprites/characters/normal.txt");
  frame = frame_list[0];
};

NormalMario::~NormalMario() {}

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
  if (v != 0) {
    frame = frame_list[8];
  }

  if (t_x != -1 && t - t_x != rT && v == 0) {
    frame = frame_list[(t - t_x) / (rT / 2) + 3];
  }

  if (left) {
    frame.width = -abs(frame.width);
  } else
    frame.width = abs(frame.width);
}

void NormalMario::Draw() {
  if (disabled) {
    if (t / 5 % 2 == 0) {
      if (left) {
        frame.width = -abs(frame.width);
      } else
        frame.width = abs(frame.width);
      DrawTextureRec(texture, frame, pos, WHITE);
    } else {
      if (left) {
        pre_frame_.width = -abs(pre_frame_.width);
      } else
        pre_frame_.width = abs(pre_frame_.width);
      DrawTextureRec(pre_texture_, pre_frame_, pos, WHITE);
    }
    if (t == 30) {
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

BigMario::BigMario(Character *Ncontext, float Nscale, bool Nleft, bool tran)
    : CharacterState(15, 10, 50, 300, Nscale, Nleft) {
  disabled    = true;
  Image image = LoadImage("res/sprites/characters/mario_starup.png");
  ImageResize(&image, image.width * scale, image.height * scale);
  texture = LoadTextureFromImage(image);
  UnloadImage(image);
  context = Ncontext;
  LoadFrameList("res/sprites/characters/starup.txt");
  frame = frame_list[0];

  if (tran) {
    image      = LoadImage("res/sprites/characters/mario_normal.png");
    pre_frame_ = {0, 0, 12 * scale, 15 * scale};
  } else {
    image      = LoadImage("res/sprites/characters/mario_starup_fire.png");
    pre_frame_ = frame_list[0];
  }
  ImageResize(&image, image.width * scale, image.height * scale);
  pre_texture_ = LoadTextureFromImage(image);
  UnloadImage(image);
}

BigMario::~BigMario() {}

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
  float v0 = 4.0 * jH / jT;
  if (v != 0) {
    if (v < -v0 * 1 / 3) {
      frame = frame_list[6];
    } else if (v > v0 * 1 / 3) {
      frame = frame_list[8];
    } else {
      frame = frame_list[7];
    }
  }

  if (t_x != -1 && t - t_x != rT && v == 0) {
    frame = frame_list[(t - t_x) / (rT / 3) + 3];
  }

  if (left) {
    frame.width = -abs(frame.width);
  } else
    frame.width = abs(frame.width);
}

void BigMario::Draw() {
  if (t <= 30) {
    if (t / 5 % 2 == 0) {
      if (left) {
        frame.width = -abs(frame.width);
      } else
        frame.width = abs(frame.width);
      DrawTextureRec(texture, frame, pos, WHITE);
    } else {
      if (left) {
        pre_frame_.width = -abs(pre_frame_.width);
      } else
        pre_frame_.width = abs(pre_frame_.width);
      DrawTextureRec(pre_texture_, pre_frame_, pos, WHITE);
    }
    if (t == 30)
      UnloadTexture(pre_texture_);
  } else {
    disabled = false;
    CharacterState::Draw();
  }
}

//+----------------------------------------------------------+
//|                        Fire Mario                        |
//+----------------------------------------------------------+

FireMario::FireMario(Character *Ncontext, float Nscale, bool Nleft, bool tran)
    : CharacterState(15, 10, 50, 300, Nscale, Nleft) {
  LoadFrameList("res/sprites/characters/starup.txt");
  if (tran) {
    disabled    = true;
    Image image = LoadImage("res/sprites/characters/mario_starup.png");
    ImageResize(&image, image.width * scale, image.height * scale);
    pre_texture_ = LoadTextureFromImage(image);
    UnloadImage(image);
    pre_frame_ = frame_list[0];
  }
  Image image = LoadImage("res/sprites/characters/mario_starup_fire.png");
  ImageResize(&image, image.width * scale, image.height * scale);
  texture = LoadTextureFromImage(image);
  UnloadImage(image);
  context = Ncontext;
  frame   = frame_list[0];
};

FireMario::~FireMario() {}

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
  float v0 = 4.0 * jH / jT;
  if (v != 0) {
    if (v < -v0 * 1 / 3) {
      frame = frame_list[6];
    } else if (v > v0 * 1 / 3) {
      frame = frame_list[8];
    } else {
      frame = frame_list[7];
    }
  }

  if (t_x != -1 && t - t_x != rT && v == 0) {
    frame = frame_list[(t - t_x) / (rT / 3) + 3];
  }

  if (left) {
    frame.width = -abs(frame.width);
  } else
    frame.width = abs(frame.width);
}

void FireMario::Draw() {
  if (disabled) {
    if (t / 5 % 2 == 0) {
      if (left) {
        frame.width = -abs(frame.width);
      } else
        frame.width = abs(frame.width);
      DrawTextureRec(texture, frame, pos, WHITE);
    } else {
      if (left) {
        pre_frame_.width = -abs(pre_frame_.width);
      } else
        pre_frame_.width = abs(pre_frame_.width);
      DrawTextureRec(pre_texture_, pre_frame_, pos, WHITE);
    }
    if (t == 30) {
      disabled = false;
      UnloadTexture(pre_texture_);
    }
  } else {
    CharacterState::Draw();
  }
}
