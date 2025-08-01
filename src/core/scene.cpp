#include "include/core/scene.hpp"

void Scene::SetVisible(bool visible) {
  visible_ = visible;
}

bool Scene::IsVisible() const {
  return visible_;
}