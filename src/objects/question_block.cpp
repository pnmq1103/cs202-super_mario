#include "include/objects/question_block.hpp"
#include "include/objects/object_manager.hpp"

QuestionBlock::QuestionBlock(
  Vector2 Nposition, float Nscale, QuestionBlockItem Nitem)
    : GameObject(Nposition, Nscale) {
  item_         = Nitem;
  texture       = &App.Resource().GetObject();
  is_used_      = false;
  item_spawned_ = false;
  LoadFrameList("res/sprites/objects/objects.txt");
}

QuestionBlock::~QuestionBlock() {}

void QuestionBlock::OnHit() {
  if (is_used_)
    return;
  is_used_ = true;
  Bounce();
}
void QuestionBlock::Update() {
  if (is_used_ && velocity.y == 0 && position.y == y_mark) {

    if (!item_spawned_) {
      item_spawned_ = true;
      texture       = &App.Resource().GetObject();
      frame         = {110, 88, 16, 16};
      switch (item_) {
        case coin:
          ObjectManager::GetInstance().AddCoin(
            {position.x + 2 * scale, position.y - 16 * scale});
          break;
        case fire_flower:
          ObjectManager::GetInstance().AddFireFlower(
            {position.x, position.y - 16 * scale});
          break;
        case super_mushroom:
          ObjectManager::GetInstance().AddSuperMushroom(
            {position.x, position.y - 16 * scale}, false);
          break;
        case super_star:
          ObjectManager::GetInstance().AddSuperStar(
            {position.x, position.y - 16 * scale});
          break;
      }
    }

  } else
    frame = frame_list[(time / 5) % 4];

  GameObject::Update();
}

ObjectType QuestionBlock::GetType() {
  return ObjectType::Block;
}