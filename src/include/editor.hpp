#pragma once

#include "scene.hpp"

class EditorScene : public Scene {
private:
  static constexpr int blockSize = 16 * 4;

  Rectangle player
    = {screenWidth / 2.0f, screenHeight / 2.0f, blockSize, blockSize};

public:
  EditorScene();
  ~EditorScene();

  void Init() override;
  void Update() override;
  void Draw() override;

  void LoadFile();
};

// Since the original game's resolution is 256x240 pixels
// and each block is 16x16 pixels.
// The map should be 16x15 blocks.