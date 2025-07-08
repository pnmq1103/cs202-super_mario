#pragma once
#include <memory>
#include <stack>

#include "scene.hpp"

class SceneStack {
private:
  std::stack<std::unique_ptr<Scene>> scenes_;

public:
  bool Push(std::unique_ptr<Scene> scene);
  bool Pop();
  Scene &Top();

  void Init();
  void Update();
  void Draw();

  size_t Size();
};