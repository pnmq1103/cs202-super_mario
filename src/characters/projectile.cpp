#include "../include/characters/projectile.hpp"

Projectile::Projectile(float Nscale, Vector2 Npos, Vector2 Nv) : scale(Nscale) {
  ori_pos = pos = Npos;
  v             = Nv;
  bounce_count  = 0;
  destroy       = false;
  t             = 0;
}
Projectile::~Projectile() {
  UnloadTexture(texture);
}
void Projectile::LoadFrameList(std::string at) {
  std::ifstream fin;
  fin.open(at);
  if (!fin.is_open())
    throw std::runtime_error("cannot open file");
  while (!fin.eof()) {
    Rectangle rect;
    float n;
    fin >> n;
    fin >> n;
    rect.x = n * scale;
    fin >> n;
    rect.y = n * scale;
    fin >> n;
    rect.width = n * scale;
    fin >> n;
    rect.height = n * scale;
    frame_list.push_back(rect);
  }
  fin.close();
}
void Projectile::SetFrameCount() {
  ++t;
}
void Projectile::Draw() {
  DrawTextureRec(texture, frame, pos, WHITE);
}
Rectangle Projectile::GetRectangle() {
  return {pos.x, pos.y, frame.width, frame.height};
}
void Projectile::Destroy() {
  destroy = true;
}