#pragma once
#include "include/objects/game_object.hpp"

class PipeBlock : public GameObject {
private:
  bool is_vertical_, has_head_, has_tail_;
  int length_;

  Rectangle MakeDestRectMod(Vector2 pos, Rectangle rect);

public:
  PipeBlock(
    Vector2 Nposition, float Nscale, int Nlength, bool Nis_vertical,
    bool Nhas_head, bool Nhas_tail);
  ~PipeBlock();
  Rectangle GetRectangle();
  void OnHit();
  void Draw();
  bool IsPipe();
  ObjectType GetType();
};