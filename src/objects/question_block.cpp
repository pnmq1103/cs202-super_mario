#include "include/objects/question_block.hpp"
QuestionBlock::QuestionBlock(Vector2 Nposition, float Nscale)
    : GameObject(Nposition, Nscale) {
  texture  = &App.Resource().GetObject();
  is_used_ = false;
  LoadFrameList("res/sprites/icons/objects.txt");
}

QuestionBlock::~QuestionBlock() {}

void QuestionBlock::OnHit() {
  if (is_used_)
    return;
  is_used_ = true;
  Bounce();
}
void QuestionBlock::Update() {
  if (is_used_ && velocity.y == 0) {
    texture = &App.Resource().GetIcon();
    frame   = {110, 88, 16, 16};
  } else
    frame = frame_list[(time / 5) % 4];
  GameObject::Update();
}