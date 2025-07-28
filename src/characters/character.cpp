#include <climits>
#include <cmath>
#include <raylib.h>
#include <stdexcept>

#include "include/characters/character.hpp"
#include "include/characters/luigi.hpp"
#include "include/characters/mario.hpp"

Character::Character(float Nscale) : scale_(Nscale) {}

Character::~Character() {
  if (pState_)
    delete pState_;
}

void Character::SetCharacter(CharacterType Ntype, Vector2 position) {
  type_ = Ntype;
  SetState(0, true);
  pState_->SetPosition(position);
}

void Character::SetState(int state, bool is_evolving) {
  Vector2 position = {INT_MIN, INT_MIN};
  Rectangle rect   = {0, 0, 0, 0};
  bool left        = false;
  if (pState_) {
    rect       = pState_->GetRectangle();
    position.x = rect.x;
    position.y = rect.y;
    left       = pState_->to_left;
    delete pState_;
  }
  if (type_ == MARIO) {
    switch (state) {
      case 0:
        pState_ = new NormalMario(this, scale_, left, is_evolving);
        break;
      case 1:
        pState_ = new BigMario(this, scale_, left, is_evolving);
        break;
      case 2:
        pState_ = new FireMario(this, scale_, left, is_evolving);
        break;
      default:
        throw std::runtime_error("invalid state");
        break;
    }
  } else if (type_ == LUIGI) {
    switch (state) {
      case 0:
        pState_ = new NormalLuigi(this, scale_, left, is_evolving);
        break;
      case 1:
        pState_ = new BigLuigi(this, scale_, left, is_evolving);
        break;
      case 2:
        pState_ = new FireLuigi(this, scale_, left, is_evolving);
        break;
      default:
        throw std::runtime_error("invalid state");
        break;
    }
  }
  if (position.x > INT_MIN && position.y > INT_MIN) {
    Rectangle rect2 = pState_->GetRectangle();
    if (rect.width > 0) {
      position.x -= fabs(rect2.width) - fabs(rect.width);
    }
    pState_->SetPosition(position);
  }
}

CharacterType Character::GetCharacter() {
  return type_;
}

int Character::GetState() {
  return state_;
}

void Character::Draw() {
  if (!pState_)
    throw std::runtime_error("not yet set character");
  pState_->Draw();
}

void Character::Run(bool to_left) {
  if (!pState_)
    throw std::runtime_error("not yet set character");
  pState_->Run(to_left);
}

void Character::Jump() {
  if (!pState_)
    throw std::runtime_error("not yet set character");
  pState_->Jump();
}

void Character::Update() {
  if (!pState_)
    throw std::runtime_error("not yet set character");
  pState_->Update();
}

void Character::SetFrameCount() {
  if (!pState_)
    throw std::runtime_error("not yet set character");
  pState_->SetFrameCount();
}

void Character::StopX() {
  if (!pState_)
    throw std::runtime_error("not yet set character");
  pState_->StopX();
}

void Character::StopY(float Ny) {
  if (!pState_)
    throw std::runtime_error("not yet set character");
  pState_->StopY(Ny);
}

void Character::StopY() {
  if (!pState_)
    throw std::runtime_error("not yet set character");
  pState_->StopY();
}

Rectangle Character::GetRectangle() {
  if (!pState_)
    throw std::runtime_error("not yet set character");
  return pState_->GetRectangle();
}

Vector2 Character::GetSpeed() {
  if (!pState_)
    throw std::runtime_error("not yet set character");
  return pState_->GetSpeed();
}

bool Character::IsFalling() {
  if (!pState_)
    throw std::runtime_error("not yet set character");
  return pState_->IsFalling();
}

void Character::Die() {
  if (!pState_)
    throw std::runtime_error("not yet set character");
  pState_->Die();
}

void Character::Evolve() {
  if (!pState_)
    throw std::runtime_error("not yet set character");
  if (state_ < 4)
    pState_->Evolve();
}

void Character::ToStarman() {
  if (!pState_)
    throw std::runtime_error("not yet set character");
  pState_->ToStarman();
}

bool Character::IsStarman() {
  if (!pState_)
    throw std::runtime_error("not yet set character");
  return pState_->IsStarman();
}

void Character::Bounce() {
  if (!pState_)
    throw std::runtime_error("not yet set character");
  pState_->Bounce();
}

bool Character::IsToLeft() {
  if (!pState_)
    throw std::runtime_error("not yet set character");
  return pState_->IsToLeft();
}