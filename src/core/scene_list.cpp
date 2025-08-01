#include <memory>
#include <stdexcept>

#include "include/core/scene.hpp"
#include "include/core/scene_list.hpp"

void SceneStack::Push(std::unique_ptr<Scene> scene) {
  if (scene == nullptr)
    throw std::runtime_error("push nullptr");

  scenes_.push_back(std::move(scene));
}

void SceneStack::Pop() {
  if (scenes_.empty())
    throw std ::runtime_error("remove empty list");

  scenes_.pop_back();
}

Scene &SceneStack::Top() {
  if (scenes_.empty())
    throw std::runtime_error("access empty list");

  return *scenes_.back();
}

size_t SceneStack::Size() {
  return scenes_.size();
}

void SceneStack::Init() {
  if (!scenes_.empty())
    scenes_.back()->Init();
}

void SceneStack::Update() {
  if (!scenes_.empty())
    scenes_.back()->Update();
}

void SceneStack::Draw() {
  for (auto &scene : scenes_) {
    if (scene->IsVisible())
      scene->Draw();
  }
}