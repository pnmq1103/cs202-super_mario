#include "include/objects/object_manager.hpp"

ObjectManager::ObjectManager() {}

ObjectManager &ObjectManager::GetInstance() {
  static ObjectManager instance;
  return instance;
}

ObjectManager::~ObjectManager() {
  for (int i = 0; i < object_list_.size(); ++i) {
    if (object_list_[i])
      delete object_list_[i];
  }
}

void ObjectManager::Reset(int Nscale, CollisionHandler *Ncollision) {
  GameObject::Reset();
  scale_ = Nscale;
  for (int i = 0; i < object_list_.size(); ++i) {
    if (object_list_[i])
      delete object_list_[i];
  }
  object_list_.clear();
  collision_ = Ncollision;
}

void ObjectManager::DeleteObject(int index) {
  collision_->RemoveObject(index);
  if (object_list_[index])
    delete object_list_[index];
  object_list_[index] = nullptr;
}

void ObjectManager::AddBrickBlock(Vector2 position) {
  GameObject *temp = new BrickBlock(position, scale_);
  object_list_.push_back(temp);
  collision_->AddObject(temp);
}

void ObjectManager::AddCoin(Vector2 position) {
  GameObject *temp = new Coin(position, scale_);
  object_list_.push_back(temp);
  collision_->AddObject(temp);
}

void ObjectManager::AddFireFlower(Vector2 position) {
  GameObject *temp = new FireFlower(position, scale_);
  object_list_.push_back(temp);
  collision_->AddObject(temp);
}

void ObjectManager::AddPipeBlock(
  Vector2 position, int length, bool is_vertical, bool has_head,
  bool has_tail) {
  GameObject *temp
    = new PipeBlock(position, scale_, length, is_vertical, has_head, has_tail);
  object_list_.push_back(temp);
  collision_->AddObject(temp);
}

void ObjectManager::AddQuestionBlock(Vector2 position, QuestionBlockItem item) {
  GameObject *temp = new QuestionBlock(position, scale_, item);
  object_list_.push_back(temp);
  collision_->AddObject(temp);
}

void ObjectManager::AddStaticBlock(Vector2 position, char theme) {
  GameObject *temp = new StaticBlock(position, scale_, theme);
  object_list_.push_back(temp);
  collision_->AddObject(temp);
}

void ObjectManager::AddSuperMushroom(Vector2 position, bool to_left) {
  GameObject *temp = new SuperMushroom(position, scale_, to_left);
  object_list_.push_back(temp);
  collision_->AddObject(temp);
}

void ObjectManager::AddSuperStar(Vector2 position) {
  GameObject *temp = new SuperStar(position, scale_);
  object_list_.push_back(temp);
  collision_->AddObject(temp);
}

void ObjectManager::SetFrameCount() {
  GameObject::SetFrameCount();
}

void ObjectManager::Update() {
  for (int i = 0; i < object_list_.size(); ++i) {
    if (object_list_[i])
      object_list_[i]->Update();
  }
}

void ObjectManager::Draw() {
  for (int i = 0; i < object_list_.size(); ++i) {
    if (object_list_[i])
      object_list_[i]->Draw();
  }
}

void ObjectManager::OnHit(int index) {
  object_list_[index]->OnHit();
}