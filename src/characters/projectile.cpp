#include "include/characters/projectile.hpp"

Projectile::Projectile(float Nscale, Vector2 Nvelocity) : scale(Nscale) {
  velocity   = Nvelocity;
  is_destroy = true;
  time       = 0;
}
Projectile::~Projectile() {}
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
    rect.x = n;
    fin >> n;
    rect.y = n;
    fin >> n;
    rect.width = n;
    fin >> n;
    rect.height = n;
    frame_list.push_back(rect);
  }
  fin.close();
}

Rectangle Projectile::MakeDestRect(Rectangle rectangle) {
  return {
    position.x, position.y, rectangle.width * scale, rectangle.height * scale};
}

void Projectile::SetFrameCount() {
  ++time;
}
void Projectile::Draw() {
  DrawTexturePro(*texture, frame, MakeDestRect(frame), {0, 0}, 0.f, WHITE);
}
Rectangle Projectile::GetRectangle() {
  return {position.x, position.y, frame.width * scale, frame.height * scale};
}
void Projectile::Destroy() {
  is_destroy = true;
}
bool Projectile::IsDestroyed() {
  return is_destroy;
}
void Projectile::Renew(Vector2 Nposition, bool to_left) {
  original_position = position = Nposition;
  is_destroy                   = false;
  time                         = 0;
  if (to_left)
    velocity.x *= -1;
  time = 0;
}