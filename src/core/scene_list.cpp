#include <memory>
#include <stdexcept>

#include "include/core/scene.hpp"
#include "include/core/scene_list.hpp"

SceneList::SceneList() {
  scenes_.reserve(10);
}

void SceneList::Push(std::unique_ptr<Scene> scene) {
  if (scene == nullptr)
    throw std::runtime_error("push nullptr");

  scenes_.push_back(std::move(scene));
}

void SceneList::Pop() {
  if (scenes_.empty())
    throw std ::runtime_error("remove empty list");

  scenes_.pop_back();
}

Scene &SceneList::Top() {
  if (scenes_.empty())
    throw std::runtime_error("access empty list");

  return *scenes_.back();
}

Scene &SceneList::Second() {
  if (scenes_.empty())
    throw std::runtime_error("access empty list");

  if (scenes_.size() < 2)
    throw std::runtime_error("invalid index");

  return *scenes_[scenes_.size() - 2];
}

size_t SceneList::Size() {
  return scenes_.size();
}

void SceneList::Init() {
  if (!scenes_.empty())
    scenes_.back()->Init();
}

void SceneList::Update() {
  if (!scenes_.empty())
    scenes_.back()->Update();
}

void SceneList::Draw() {
  for (auto &scene : scenes_) {
    if (scene->IsVisible())
      scene->Draw();
  }
}