#pragma once
#include <raylib.h>

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
  CharacterSelector,
  Pause,
  GameOver
};

class Scene {
protected:
  Camera2D camera_         = {};
  Vector2 mouse_world_pos_ = {0, 0};
  bool visible_            = {true};

public:
  virtual ~Scene() = default;

  virtual void Init()      = 0;
  virtual void Update()    = 0;
  virtual void Draw()      = 0;
  virtual void Resume()    = 0;
  virtual SceneType Type() = 0;

  void SetVisible(bool visible);
  bool IsVisible() const;
};

// Constructor should be used to initialize members only