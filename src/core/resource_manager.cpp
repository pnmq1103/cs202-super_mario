#include <array>
#include <stdexcept>

#include "include/core/resource_manager.hpp"

ResManager::~ResManager() {
  for (auto &texture : textures_)
    UnloadTexture(texture.second);

  for (auto &sound : sounds)
    UnloadSound(sound.second);

  for (auto &music : musics) {
    StopMusicStream(music.second);
    UnloadMusicStream(music.second);
  }
}

ResManager::ResManager() {}

void ResManager::Init() {
  LoadTextures();
  LoadSounds();
  LoadMusic();
}

void ResManager::LoadTextures() {
  textures_["mario_normal"]
    = LoadTexture("res/sprites/characters/mario_normal.png");
  textures_["mario_star"]
    = LoadTexture("res/sprites/characters/mario_star.png");
  textures_["mario_fire"]
    = LoadTexture("res/sprites/characters/mario_fire.png");

  textures_["luigi_normal"]
    = LoadTexture("res/sprites/characters/luigi_normal.png");
  textures_["luigi_star"]
    = LoadTexture("res/sprites/characters/luigi_star.png");
  textures_["luigi_fire"]
    = LoadTexture("res/sprites/characters/luigi_fire.png");
  textures_["luigi_electric"]
    = LoadTexture("res/sprites/characters/luigi_electric.png");

  textures_["enemies"] = LoadTexture("res/sprites/enemies/enemies.png");
  textures_["icons"]   = LoadTexture("res/sprites/icons/icons.png");
  textures_["objects"] = LoadTexture("res/sprites/icons/objects.png");

  textures_["tileset_ground"]
    = LoadTexture("res/sprites/tilesets/tileset_ground.png");
  textures_["tileset_sky"]
    = LoadTexture("res/sprites/tilesets/tileset_sky.png");
  textures_["tileset_underground"]
    = LoadTexture("res/sprites/tilesets/tileset_underground.png");
  textures_["tileset_water"]
    = LoadTexture("res/sprites/tilesets/tileset_underwater.png");

  textures_["backgrounds"]
    = LoadTexture("res/sprites/backgrounds/backgrounds.png");
}

void ResManager::LoadMusic() {
  auto Load = [this](const std::string &name) {
    musics[name] = LoadMusicStream(("res/musics/" + name + ".ogg").c_str());
  };

  for (const auto &music : music_names)
    Load(music);
}

void ResManager::LoadSounds() {
  auto Load = [this](const std::string &name) {
    sounds[name] = LoadSound(("res/sounds/" + name + ".wav").c_str());
  };

  for (const auto &sound : sound_names)
    Load(sound);
}

// SaveData ResManager::LoadResourcesFromFile() {
//   FileHandler fp;
//   std::string path = fp.OpenFilePath();
//   if (path.empty()) {
//     return {};
//   }
//   SaveData data;
//   if (!fp.LoadFile(path, data)) {
//     throw std::runtime_error("Failed to load file at: " + path);
//   }
//   return data;
// }
//
// bool ResManager::SaveResourcesToFile(const SaveData &data) {
//   FileHandler fp;
//   std::string path = fp.OpenSavePath();
//   if (path.empty()) {
//     return false;
//   }
//   return fp.SaveFile(path, data);
// }

const Texture &ResManager::GetMario(char type) const {
  switch (type) {
    case 'n':
      return textures_.at("mario_normal");
    case 's':
      return textures_.at("mario_star");
    case 'f':
      return textures_.at("mario_fire");
    default:
      throw std::runtime_error("Invalid Mario type");
  }
}

const Texture &ResManager::GetLuigi(char type) const {
  switch (type) {
    case 'n':
      return textures_.at("luigi_normal");
    case 's':
      return textures_.at("luigi_star");
    case 'f':
      return textures_.at("luigi_fire");
    case 'e':
      return textures_.at("luigi_electric");
    default:
      throw std::runtime_error("Invalid Luigi type");
  }
}

const Texture &ResManager::GetEnemy() const {
  return textures_.at("enemies");
}

const Texture &ResManager::GetTileset(char type) const {
  switch (type) {
    case 'g':
      return textures_.at("tileset_ground");
    case 's':
      return textures_.at("tileset_sky");
    case 'w':
      return textures_.at("tileset_water");
    case 'u':
      return textures_.at("tileset_underground");
    default:
      throw std::runtime_error("Invalid tileset type");
  }
}

const Texture &ResManager::GetIcon() const {
  return textures_.at("icons");
}

const Texture &ResManager::GetBackground() const {
  return textures_.at("backgrounds");
}

const Music &ResManager::GetMusic(std::string key) const {
  auto it = musics.find(key);
  if (it == musics.end())
    throw std::out_of_range("Missing music");
  return it->second;
}

const Sound &ResManager::GetSound(std::string key) const {
  auto it = sounds.find(key);
  if (it == sounds.end())
    throw std::out_of_range("Missing sound");
  return it->second;
}

// bool
// ResManager::LoadMap(const std::string &path, std::vector<Block *> &blockData)
// {
//   tson::Tileson t;
//   std::unique_ptr<tson::Map> map = t.parse(path);
//   if (map->getStatus() == tson::ParseStatus::OK) {
//     // get map dimension
//     int mWidth  = map->getTileSize().x;
//     int mHeight = map->getTileSize().y;
//     // for when more than one tileset is included in the map
//     struct TilesetInfo {
//       int firstGid, columns, margin, spacing;
//       Vector2 tileSize;
//     };
//     std::vector<TilesetInfo> tsi;
//     // get all tilesets for convenience
//     for (auto &ts : map->getTilesets()) {
//       std::string texPath = ts.getImagePath().string();
//       Texture tex         = LoadTexture(texPath.c_str());
//
//       int margin  = ts.getMargin();
//       int spacing = ts.getSpacing();
//       int tileW   = ts.getTileSize().x;
//       int tileH   = ts.getTileSize().y;
//       int cols    = (tex.width - 2 * margin + spacing) / (tileW + spacing);
//       // mark each texture with its first Gid
//       tilesetMapStore[ts.getFirstgid()] = tex;
//       tsi.push_back(
//         {ts.getFirstgid(),
//          cols,
//          margin,
//          spacing,
//          {float(tileW), float(tileH)}});
//     }
//     // sort the tileset according to Gid
//     std::sort(tsi.begin(), tsi.end(), [](auto &a, auto &b) {
//       return a.firstGid < b.firstGid;
//     });
//     // loop over layers
//     for (auto &layer : map->getLayers()) {
//       switch (layer.getType()) {
//         case tson::LayerType::TileLayer: {
//           if (!map->isInfinite()) {
//             std::map<std::tuple<int, int>, tson::Tile *> tData
//               = layer.getTileData();
//             for (const auto &[id, tile] : tData) {
//               Block *b;
//               b->SetSpriteId(tile->getGid());
//               b->SetSize(tile->getTileSize().x, tile->getTileSize().y);
//               b->SetPosition(
//                 {tile->getPosition(id).x, tile->getPosition(id).y});
//               b->SetType(tile->getType());
//               blockData.push_back(b);
//             }
//           }
//         }
//       }
//     }
//   }
// }
