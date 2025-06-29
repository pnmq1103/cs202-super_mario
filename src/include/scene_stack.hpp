#pragma once
#include <stack>
#include <memory>

#include "scene.hpp"

class SceneStack {
private:
  std::stack<std::unique_ptr<Scene>> stack_;

public:
  bool Push(std::unique_ptr<Scene> scene);
  bool Pop();
  void Init();
  void Update();
  void Draw();
};