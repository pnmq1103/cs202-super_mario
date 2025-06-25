#pragma once

#include "scene.hpp"

class DesignScene : public Scene {
private:
public:
  DesignScene();
  ~DesignScene();

  void Init() override;
  void Update() override;
  void Draw() override;
};
