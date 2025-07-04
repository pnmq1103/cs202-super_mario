#include <memory>
#include <utility>

#include "../include/core/scene.hpp"
#include "../include/core/scene_stack.hpp"

bool SceneStack::Push(std::unique_ptr<Scene> scene) {
  if (scene == nullptr)
    return false;

  scenes_.push(std::move(scene));
  return true;
}

bool SceneStack::Pop() {
  if (scenes_.empty())
    return false;

  scenes_.pop();
  return true;
}

void SceneStack::Init() {
  if (!scenes_.empty())
    scenes_.top()->Init();
}

void SceneStack::Update() {
  if (!scenes_.empty())
    scenes_.top()->Update();
}

void SceneStack::Draw() {
  if (!scenes_.empty())
    scenes_.top()->Draw();
}
