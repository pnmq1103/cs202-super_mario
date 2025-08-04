#pragma once
#include "include/objects/game_object.hpp"

class StaticBlock : public GameObject {
private:
public:
	//add a constructor that draws using the global id
  StaticBlock(Vector2 Nposition, float Nscale, Rectangle rect);
  //keep the constructor that draws based on the theme as fallback
  StaticBlock(Vector2 Nposition, float Nscale, char theme);
  ~StaticBlock();
  void OnHit();
  ObjectType GetType();
};