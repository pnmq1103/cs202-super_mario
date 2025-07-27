#include "include/objects/pipe_block.hpp"

Rectangle PipeBlock::MakeDestRectMod(Vector2 pos, Rectangle rect) {
  return {pos.x, pos.y, rect.width * scale, rect.height * scale};
}

PipeBlock::PipeBlock(
  Vector2 Nposition, float Nscale, int Nlength, bool Nis_vertical,
  bool Nhas_head, bool Nhas_tail)
    : GameObject(Nposition, Nscale) {
  if (Nlength < 0)
    throw std::runtime_error("invalid length for pipe block");
  length_      = Nlength;
  is_vertical_ = Nis_vertical;
  has_head_    = Nhas_head;
  has_tail_    = Nhas_tail;

  texture = &App.Resource().GetTileset('w');
  LoadFrameList("res/sprites/tilesets/tileset_underwater.txt");
}

PipeBlock::~PipeBlock() {}

Rectangle PipeBlock::GetRectangle() {
  Rectangle rect;
  rect.x      = position.x;
  rect.y      = position.y;
  rect.width  = 0;
  rect.height = 0;
  if (is_vertical_) {
    rect.width = 16 * scale * 2;
    if (has_head_)
      rect.height += 16 * scale;
    if (has_tail_)
      rect.height += 16 * scale;
    rect.height += 16 * scale * length_;
  } else {
    rect.height = 16 * scale * 2;
    if (has_head_)
      rect.width += 16 * scale;
    if (has_tail_)
      rect.width += 16 * scale;
    rect.width += 16 * scale * length_;
  }
  return rect;
}

void PipeBlock::OnHit() {}

void PipeBlock::Draw() {
  if (is_vertical_) {

    if (has_head_) {
      DrawTexturePro(
        *texture, frame_list[12], MakeDestRectMod(position, frame_list[12]),
        {0, 0}, 0.f, WHITE);

      Vector2 point = {position.x + 16 * scale, position.y};
      DrawTexturePro(
        *texture, frame_list[13], MakeDestRectMod(point, frame_list[13]),
        {0, 0}, 0.f, WHITE);
    }

    for (int i = 0; i < length_; ++i) {

      Vector2 point = {position.x + scale, position.y + (i + 1) * 16 * scale};
      DrawTexturePro(
        *texture, frame_list[26], MakeDestRectMod(point, frame_list[26]),
        {0, 0}, 0.f, WHITE);

      point = {position.x + scale * 16, position.y + (i + 1) * 16 * scale};
      DrawTexturePro(
        *texture, frame_list[27], MakeDestRectMod(point, frame_list[27]),
        {0, 0}, 0.f, WHITE);
    }

    if (has_tail_) {

      Vector2 point = {position.x, position.y + (length_ + 1) * 16 * scale};
      DrawTexturePro(
        *texture, frame_list[42], MakeDestRectMod(point, frame_list[42]),
        {0, 0}, 0.f, WHITE);

      point
        = {position.x + scale * 16, position.y + (length_ + 1) * 16 * scale};
      DrawTexturePro(
        *texture, frame_list[43], MakeDestRectMod(point, frame_list[43]),
        {0, 0}, 0.f, WHITE);
    }

  } else {

    if (has_head_) {
      DrawTexturePro(
        *texture, frame_list[10], MakeDestRectMod(position, frame_list[10]),
        {0, 0}, 0.f, WHITE);

      Vector2 point = {position.x, position.y + 16 * scale};
      DrawTexturePro(
        *texture, frame_list[23], MakeDestRectMod(point, frame_list[23]),
        {0, 0}, 0.f, WHITE);
    }

    for (int i = 0; i < length_; ++i) {

      Vector2 point = {position.x + (i + 1) * 16 * scale, position.y + scale};
      DrawTexturePro(
        *texture, frame_list[14], MakeDestRectMod(point, frame_list[14]),
        {0, 0}, 0.f, WHITE);

      point = {position.x + (i + 1) * 16 * scale, position.y + scale * 16};
      DrawTexturePro(
        *texture, frame_list[24], MakeDestRectMod(point, frame_list[24]),
        {0, 0}, 0.f, WHITE);
    }

    if (has_tail_) {

      Vector2 point = {position.x + (length_ + 1) * 16 * scale, position.y};
      DrawTexturePro(
        *texture, frame_list[11], MakeDestRectMod(point, frame_list[11]),
        {0, 0}, 0.f, WHITE);

      point
        = {position.x + (length_ + 1) * 16 * scale, position.y + scale * 16};
      DrawTexturePro(
        *texture, frame_list[25], MakeDestRectMod(point, frame_list[25]),
        {0, 0}, 0.f, WHITE);
    }
  }
}