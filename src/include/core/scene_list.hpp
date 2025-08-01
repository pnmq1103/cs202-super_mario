#pragma once
#include <memory>
#include <vector>

#include "include/core/scene.hpp"

class SceneStack {
private:
  std::vector<std::unique_ptr<Scene>> scenes_;

public:
  void Push(std::unique_ptr<Scene> scene);
  void Pop();
  Scene &Top();
  size_t Size();

  void Init();
  void Update();
  void Draw();
};