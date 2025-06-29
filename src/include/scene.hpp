#pragma once

class Scene {
public:
  virtual ~Scene() = default;
  virtual void Init() = 0;
  virtual void Update() = 0;
  virtual void Draw() = 0;
};
