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
  Camera2D camera_         = {};
  Vector2 mouse_world_pos_ = {0, 0};

public:
  virtual ~Scene() = default;

  virtual void Init()      = 0;
  virtual void Update()    = 0;
  virtual void Draw()      = 0;
  virtual void Resume()    = 0;
  virtual SceneType Type() = 0;
};

// Constructor should be used to initialize members only
// Resources such as textures, musics, etc should be loaded in Init()