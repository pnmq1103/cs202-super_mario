#include "include/character.hpp"
#include "include/luigi.hpp"
#include "include/mario.hpp"
#include <iostream>

Character::Character(float Nscale) {
  pState_ = nullptr;
  state_  = 0;
  scale_  = Nscale;
}
Character::~Character() {
  if (pState_)
    delete pState_;
}
void Character::SetCharacter(CharacterType Ntype, Vector2 pos) {
  type_ = Ntype;
  SetState(0, true);
  pState_->SetPosition(pos);
}
void Character::SetState(int state, bool tran) {
  Vector2 pos = {INT_MIN, INT_MIN};
  Rectangle rect;
  bool left;
  if (pState_) {
    rect  = pState_->GetRectangle();
    pos.x = rect.x;
    pos.y = rect.y;
    left  = pState_->left;
    delete pState_;
  }
  if (type_ == MARIO) {
    switch (state) {
      case 0:
        pState_ = new NormalMario(this, scale_, left, tran);
        break;
      case 1:
        pState_ = new BigMario(this, scale_, left, tran);
        break;
      case 2:
        pState_ = new FireMario(this, scale_, left, tran);
        break;
      default:
        throw std::runtime_error("invalid state");
        break;
    }
  } else if (type_ == LUIGI) {
    switch (state) {
      case 0:
        pState_ = new NormalLuigi(this, scale_, left, tran);
        break;
      case 1:
        pState_ = new BigLuigi(this, scale_, left, tran);
        break;
      case 2:
        pState_ = new FireLuigi(this, scale_, left, tran);
        break;
      default:
        throw std::runtime_error("invalid state");
        break;
    }
  }
  if (pos.x > INT_MIN && pos.y > INT_MIN)
    pState_->SetPosition(pos);
}
CharacterType Character::GetCharacter() {
  return type_;
}
int Character::GetState() {
  return state_;
}

void Character::Draw() {
  pState_->Draw();
}
void Character::Run(bool left) {
  pState_->Run(left);
}
void Character::Jump() {
  pState_->Jump();
}
void Character::Update() {
  pState_->Update();
}
void Character::SetFrameCount() {
  pState_->SetFrameCount();
}
void Character::StopX() {
  pState_->StopX();
}
void Character::StopY(float Ny) {
  pState_->StopY(Ny);
}
void Character::StopY() {
  pState_->StopY();
}
Rectangle Character::GetRectangle() {
  return pState_->GetRectangle();
}
float Character::GetSpeed() {
  return pState_->GetSpeed();
}
bool Character::IsFalling() {
  return pState_->IsFalling();
}
void Character::Die() {
  pState_->Die();
}
void Character::Evolve() {
  if (state_ < 4)
    pState_->Evolve();
}
void Character::ToStarman() {
  pState_->ToStarman();
}
bool Character::IsStarman() {
  return pState_->IsStarman();
}
