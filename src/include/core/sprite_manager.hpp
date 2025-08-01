#pragma once
#include <raylib.h>
#include <string>
#include <unordered_map>

class SpriteManager {
  std::unordered_map<std::string, Texture> textures_;

public:
  ~SpriteManager();
  SpriteManager() = default;

  void Init();

  const Texture &GetMario(char type) const;
  const Texture &GetLuigi(char type) const;
  const Texture &GetEnemy(char type) const;
  const Texture &GetTileset(char type) const;
  const Texture &GetIcon() const;
  const Texture &GetObject() const;
  const Texture &GetElectricShot() const;
  const Texture &GetBackground() const;
};