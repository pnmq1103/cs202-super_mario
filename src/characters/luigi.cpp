#include <cstdlib>
#include <raylib.h>

#include "include/characters/character.hpp"
#include "include/characters/character_state.hpp"
#include "include/characters/luigi.hpp"

//+----------------------------------------------------------+
//|                       Normal Luigi                       |
//+----------------------------------------------------------+

NormalLuigi::NormalLuigi(
  Character *Ncontext, float Nscale, bool Nto_left, bool is_evolving)
    : CharacterState(18, 7, 70, 500, Nscale, Nto_left) {
  if (!is_evolving) {
    disabled     = true;
    pre_texture_ = &App.Resource().GetLuigi('s');
    pre_frame_   = {254, 0, 14, 27};
  }
  texture = &App.Resource().GetLuigi('n');
  context = Ncontext;
  LoadFrameList("res/sprites/characters/normal.txt");
  frame = frame_list[0];
};

NormalLuigi::~NormalLuigi() {}

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

void NormalLuigi::Draw() {
  if (disabled) {
    if (time / 5 % 2 == 0) {
      if (to_left) {
        frame.width = -abs(frame.width);
      } else
        frame.width = abs(frame.width);
      DrawTexturePro(*texture, frame, MakeDestRect(frame), {0, 0}, 0.f, WHITE);
    } else {
      if (to_left) {
        pre_frame_.width = -abs(pre_frame_.width);
      } else
        pre_frame_.width = abs(pre_frame_.width);
      DrawTexturePro(
        *pre_texture_, pre_frame_, MakeDestRect(pre_frame_), {0, 0}, 0.f,
        WHITE);
    }
    if (time == 30) {
      disabled = false;
    }
  } else {
    CharacterState::Draw();
  }
}

//+----------------------------------------------------------+
//|                        Big Luigi                         |
//+----------------------------------------------------------+

BigLuigi::BigLuigi(
  Character *Ncontext, float Nscale, bool Nto_left, bool is_evolving)
    : CharacterState(18, 7, 70, 500, Nscale, Nto_left) {
  disabled = true;
  texture  = &App.Resource().GetLuigi('s');
  context  = Ncontext;
  LoadFrameList("res/sprites/characters/powerup.txt");
  frame = frame_list[0];

  if (is_evolving) {
    pre_texture_ = &App.Resource().GetLuigi('n');
    pre_frame_   = {0, 0, 12, 15};
  } else {
    pre_texture_ = &App.Resource().GetLuigi('e');
    pre_frame_   = frame_list[0];
  }
}

BigLuigi::~BigLuigi() {}

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

void BigLuigi::Draw() {
  if (time <= 30) {
    if (time / 5 % 2 == 0) {
      if (to_left) {
        frame.width = -abs(frame.width);
      } else
        frame.width = abs(frame.width);
      DrawTexturePro(*texture, frame, MakeDestRect(frame), {0, 0}, 0.f, WHITE);
    } else {
      if (to_left) {
        pre_frame_.width = -abs(pre_frame_.width);
      } else
        pre_frame_.width = abs(pre_frame_.width);
      DrawTexturePro(
        *pre_texture_, pre_frame_, MakeDestRect(pre_frame_), {0, 0}, 0.f,
        WHITE);
    }
  } else {
    disabled = false;
    CharacterState::Draw();
  }
}

//+----------------------------------------------------------+
//|                        Fire Luigi                        |
//+----------------------------------------------------------+

FireLuigi::FireLuigi(
  Character *Ncontext, float Nscale, bool Nto_left, bool is_evolving)
    : CharacterState(18, 7, 70, 500, Nscale, Nto_left) {
  LoadFrameList("res/sprites/characters/powerup.txt");
  if (is_evolving) {
    disabled     = true;
    pre_texture_ = &App.Resource().GetLuigi('s');
    pre_frame_   = frame_list[0];
  }
  texture = &App.Resource().GetLuigi('e');
  context = Ncontext;
  frame   = frame_list[0];
};

FireLuigi::~FireLuigi() {}

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

void FireLuigi::Draw() {
  if (disabled) {
    if (time / 5 % 2 == 0) {
      if (to_left) {
        frame.width = -abs(frame.width);
      } else
        frame.width = abs(frame.width);
      DrawTexturePro(*texture, frame, MakeDestRect(frame), {0, 0}, 0.f, WHITE);
    } else {
      if (to_left) {
        pre_frame_.width = -abs(pre_frame_.width);
      } else
        pre_frame_.width = abs(pre_frame_.width);
      DrawTexturePro(
        *pre_texture_, pre_frame_, MakeDestRect(pre_frame_), {0, 0}, 0.f,
        WHITE);
    }
    if (time == 30) {
      disabled = false;
    }
  } else {
    CharacterState::Draw();
  }
}
