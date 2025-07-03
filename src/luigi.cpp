#include <cstdlib>
#include <raylib.h>

#include "include/character.hpp"
#include "include/character_state.hpp"
#include "include/luigi.hpp"

//+----------------------------------------------------------+
//|                       Normal Luigi                       |
//+----------------------------------------------------------+

NormalLuigi::NormalLuigi(
  Character *Ncontext, float Nscale, bool Nleft, bool tran)
    : CharacterState(18, 5, 50, 500, Nscale, Nleft) {
  if (!tran) {
    disabled    = true;
    Image image = LoadImage("res/sprites/characters/luigi_starup.png");
    ImageResize(&image, image.width * scale, image.height * scale);
    pre_texture_ = LoadTextureFromImage(image);
    UnloadImage(image);
    pre_frame_ = {254 * scale, 0, 14 * scale, 27 * scale};
  }
  Image image = LoadImage("res/sprites/characters/luigi_normal.png");
  ImageResize(&image, image.width * scale, image.height * scale);
  texture = LoadTextureFromImage(image);
  UnloadImage(image);
  context = Ncontext;
  LoadFrameList("res/sprites/characters/normal.txt");
  frame = frame_list[0];
};

NormalLuigi::~NormalLuigi() {
  UnloadTexture(pre_texture_);
}

void NormalLuigi::Die() {
  if (disabled)
    return;
  CharacterState::Jump();
  is_die = true;
}

void NormalLuigi::Evolve() {
  if (disabled)
    return;
  context->SetState(1, true);
}

void NormalLuigi::Update() {
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

void NormalLuigi::Draw() {
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

//+----------------------------------------------------------+
//|                        Big Luigi                         |
//+----------------------------------------------------------+

BigLuigi::BigLuigi(Character *Ncontext, float Nscale, bool Nleft, bool tran)
    : CharacterState(18, 5, 50, 500, Nscale, Nleft) {
  disabled    = true;
  Image image = LoadImage("res/sprites/characters/luigi_starup.png");
  ImageResize(&image, image.width * scale, image.height * scale);
  texture = LoadTextureFromImage(image);
  UnloadImage(image);
  context = Ncontext;
  LoadFrameList("res/sprites/characters/starup.txt");
  frame = frame_list[0];

  if (tran) {
    image      = LoadImage("res/sprites/characters/luigi_normal.png");
    pre_frame_ = {0, 0, 12 * scale, 15 * scale};
  } else {
    image      = LoadImage("res/sprites/characters/luigi_starup_fire.png");
    pre_frame_ = frame_list[0];
  }
  ImageResize(&image, image.width * scale, image.height * scale);
  pre_texture_ = LoadTextureFromImage(image);
  UnloadImage(image);
}

BigLuigi::~BigLuigi() {
  UnloadTexture(pre_texture_);
}

void BigLuigi::Die() {
  if (disabled)
    return;
  context->SetState(0, false);
}
void BigLuigi::Evolve() {
  if (disabled)
    return;
  context->SetState(2, true);
}
void BigLuigi::Update() {
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

void BigLuigi::Draw() {
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
//|                        Fire Luigi                        |
//+----------------------------------------------------------+

FireLuigi::FireLuigi(Character *Ncontext, float Nscale, bool Nleft, bool tran)
    : CharacterState(18, 5, 50, 500, Nscale, Nleft) {
  LoadFrameList("res/sprites/characters/starup.txt");
  if (tran) {
    disabled    = true;
    Image image = LoadImage("res/sprites/characters/luigi_starup.png");
    ImageResize(&image, image.width * scale, image.height * scale);
    pre_texture_ = LoadTextureFromImage(image);
    UnloadImage(image);
    pre_frame_ = frame_list[0];
  }
  Image image = LoadImage("res/sprites/characters/luigi_starup_fire.png");
  ImageResize(&image, image.width * scale, image.height * scale);
  texture = LoadTextureFromImage(image);
  UnloadImage(image);
  context = Ncontext;
  frame   = frame_list[0];
};

FireLuigi::~FireLuigi() {
  UnloadTexture(pre_texture_);
}

void FireLuigi::Die() {
  if (disabled)
    return;
  context->SetState(1, false);
}

void FireLuigi::Evolve() {}

void FireLuigi::Update() {
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

void FireLuigi::Draw() {
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
