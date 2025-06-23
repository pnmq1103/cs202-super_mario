#include "include/character.hpp"
#include "include/normal_mario.hpp"
#include <iostream>

Character::Character(float Nscale) {
  pState = nullptr;
  state = 0;
  scale = Nscale;
}
Character::~Character() {
  if (pState)
    delete pState;
}
void Character::setCharacter(CharacterType Ntype, Vector2 pos) {
  type = Ntype;
  setState(0);
  pState->setPosition(pos);
}
void Character::setState(int state) {
  if (type == MARIO) {
    switch (state) {
    case 0:
      pState = new NormalMario(this, scale);
      break;
    }
  } else if (type == LUIGI) {
  }
}
void Character::draw() { pState->draw(); }
void Character::run(bool left) { pState->run(left); }
void Character::jump() { pState->jump(); }
void Character::update() { pState->update(); }
void Character::setFrameCount() { pState->setFrameCount(); }