#include "include/scene_stack.hpp"

bool SceneStack::Push(std::unique_ptr<Scene> scene) {
  if (scene == nullptr)
    return false;

  stack_.push(std::move(scene));
  return true;
}

bool SceneStack::Pop() {
  if (stack_.empty())
    return false;

  stack_.pop();
  return true;
}

void SceneStack::Init() {
  if (!stack_.empty())
    stack_.top()->Init();
}

void SceneStack::Update() {
  if (!stack_.empty())
    stack_.top()->Update();
}

void SceneStack::Draw() {
  if (!stack_.empty())
    stack_.top()->Draw();
}
