#pragma once
#include <raylib.h>
#include <string>
#include <unordered_map>

enum SceneType {
  Game,
  Load,
  Editor,
  Exit,
  Menu,
  Setting,
  Credit,
  TileSelector,
  EnemySelector,
};

class Scene {
protected:
  // The scale factor for the whole program is 4x
  static constexpr float screenWidth  = {256 * 4};
  static constexpr float screenHeight = {240 * 4};
  static constexpr float blockSize    = {16 * 4}; // int needs typecast
  static constexpr float cellSize     = {16};

  Camera2D camera_ = {};

  Vector2 mouse_world_pos_ = {0, 0};

public:
  virtual ~Scene() = default;

  virtual void Init()      = 0;
  virtual void Update()    = 0;
  virtual void Draw()      = 0;
  virtual SceneType Type() = 0;
  virtual void Resume();

  void ReadSpriteInfo(
    const std::string &path, std::unordered_map<int, Rectangle> &sprites);
};

// Constructor should be used to initialize members only
// Resources such as textures, musics, etc should be loaded in Init()
