#pragma once
#include <raylib.h>
#include <string>
#include <unordered_map>

class Scene {
protected:
  static constexpr int screenWidth  = {256 * 4};
  static constexpr int screenHeight = {240 * 4};

  Camera2D camera_ = {0};

  Vector2 mouse_world_pos_ = {0, 0};

public:
  virtual ~Scene() = default;

  virtual void Init()   = 0;
  virtual void Update() = 0;
  virtual void Draw()   = 0;

  void ReadSpriteInfo(
    const std::string &path, std::unordered_map<int, Rectangle> &sprites);
};

// Create separate texture for buttons
enum ButtonSpriteIndex {
  SETTING = 39,
  CREDIT  = 41,
};

// Constructor should be used to initialize members only
// Resources such as textures, musics, etc should be loaded in Init()
