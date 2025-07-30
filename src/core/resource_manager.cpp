#include "include/core/resource_manager.hpp"
#include <array>
#include <stdexcept>

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
  // LoadMap("res/sprite/map/map1.tmj");
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

  textures_["bowser"]  = LoadTexture("res/sprites/enemies/bowser.png");
  textures_["minions"] = LoadTexture("res/sprites/enemies/minions.png");
  textures_["enemies_icon"]
    = LoadTexture("res/sprites/enemies/enemies_icon.png");

  textures_["icons"]   = LoadTexture("res/sprites/icons/icons.png");
  textures_["objects"] = LoadTexture("res/sprites/icons/objects.png");
  textures_["electric_shot"]
    = LoadTexture("res/sprites/electric_shot/electric_shot.png");

  textures_["tileset_ground"]
    = LoadTexture("res/sprites/tilesets/tileset_ground.png");
  textures_["tileset_underground"]
    = LoadTexture("res/sprites/tilesets/tileset_underground.png");

  textures_["backgrounds"]
    = LoadTexture("res/sprites/backgrounds/background_ground.png");
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

const Texture &ResManager::GetEnemy(char type) const {
  switch (type) {
    case 'b':
      return textures_.at("bowser");
    case 'm':
      return textures_.at("minions");
    case 'i':
      return textures_.at("enemies_icon");
    default:
      throw std::runtime_error("Invalid enemy type");
  }
}

const Texture &ResManager::GetTileset(char type) const {
  switch (type) {
    case 'g':
      return textures_.at("tileset_ground");
    case 'u':
      return textures_.at("tileset_underground");
    default:
      throw std::runtime_error("Invalid tileset type");
  }
}

const Texture &ResManager::GetIcon() const {
  return textures_.at("icons");
}

const Texture &ResManager::GetObject() const {
  return textures_.at("objects");
}

const Texture &ResManager::GetElectricShot() const {
  return textures_.at("electric_shot");
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

// functions to load the map from a file using Tileson library - backup code
//
// //std::map<int, Texture> ResManager::GetTilesetMap() const {
//  return tilesetMapStore;
//}
//
// std::vector<BlockInfo> ResManager::GetBlocksMap() const {
//  return blockInfoMapStore;
//}
//
// std::vector<EnemyInfo> ResManager::GetEnemiesMap() const {
//  return enemyMapStore;
//}
//
// Texture ResManager::GetBackgroundMap() const {
//  return background;
//}
//
// void ResManager::LoadMap(const std::string &path) {
//  tson::Tileson t;
//  const auto baseDir = std::filesystem::path(path).parent_path();
//  auto mapPtr        = t.parse(path);
//
//  if (mapPtr->getStatus() != tson::ParseStatus::OK)
//    return;
//
//  tson::Map &map = *mapPtr;
//  std::vector<TilesetInfo> tsi;
//  for (auto &ts : map.getTilesets()) {
//    // resolve the image path relative to the map file
//    auto imgPath = baseDir / ts.getImagePath();
//    Texture tex  = LoadTexture(imgPath.string().c_str());
//    //get the attributes
//    int margin  = ts.getMargin();
//    int spacing = ts.getSpacing();
//    int tileW   = ts.getTileSize().x;
//    int tileH   = ts.getTileSize().y;
//    int cols    = (tex.width - 2 * margin + spacing) / (tileW + spacing);
//    //store tilesets according to their gid
//    tilesetMapStore[ts.getFirstgid()] = tex;
//    tsi.push_back(
//      {ts.getFirstgid(), cols, margin, spacing, {float(tileW),
//      float(tileH)}});
//  }
//  //sort the tileset map
//  std::sort(tsi.begin(), tsi.end(), [](auto &a, auto &b) {
//    return a.firstGid < b.firstGid;
//  });
//
//  //loop over all layers
//  for (auto &layer : map.getLayers()) {
//    switch (layer.getType()) {
//        //tile layer
//      case tson::LayerType::TileLayer: {
//        if (map.isInfinite())
//          break;
//
//        auto tData = layer.getTileData(); // map<tuple<int,int>,Tile*>
//        for (auto &kv : tData) {
//          tson::Tile *tile = kv.second;
//          BlockInfo info;
//          info.gid   = tile->getGid();
//          info.pos  = {tile->getPosition(kv.first).x,
//          tile->getPosition(kv.first).y}; info.size =
//          {static_cast<float>(tile->getTileSize().x),
//          static_cast<float>(tile->getTileSize().y)};
//          // add exception handling for properties
//          auto *propType = tile->getProp("type");
//          if (!propType)
//            throw std::runtime_error("Missing tile property 'type'");
//          if (auto p = std::any_cast<int>(&propType->getValue())) {
//            info.type = *p;
//          } else {
//            throw std::runtime_error("Tile property 'type' is not an int");
//          }
//
//          auto *propSolid = tile->getProp("solid");
//          if (!propSolid)
//            throw std::runtime_error("Missing tile property 'solid'");
//          if (auto p = std::any_cast<bool>(&propSolid->getValue())) {
//            info.solid = *p;
//          } else {
//            throw std::runtime_error("Tile property 'solid' is not a bool");
//          }
//
//          auto *propAnim = tile->getProp("animating");
//          if (!propAnim)
//            throw std::runtime_error("Missing tile property 'animating'");
//          if (auto p = std::any_cast<bool>(&propAnim->getValue())) {
//            info.animating = *p;
//          } else {
//            throw std::runtime_error("Tile property 'animating' is not a
//            bool");
//          }
//
//          blockInfoMapStore.push_back(info);
//
//          std::vector<BlockInfo> infos;
//        }
//        break;
//      }
//        //image layer (mainly to get the background)
//      case tson::LayerType::ImageLayer: {
//        // resolve against baseDir
//        auto imgPath = baseDir / layer.getImage();
//        background   = LoadTexture(imgPath.string().c_str());
//        break;
//      }
//        //object group (enemies)
//      case tson::LayerType::ObjectGroup: {
//        for (auto &obj : layer.getObjects()) {
//          EnemyInfo e;
//          e.gid = obj.getGid();
//          e.pos = {
//              static_cast<float>(obj.getPosition().x),
//              static_cast<float>(obj.getPosition().y)};
//          e.size = {
//            static_cast<float>(obj.getSize().x),
//            static_cast<float>(obj.getSize().y)};
//          // add exception handling for properties
//          auto *propEType = obj.getProp("type");
//          if (!propEType)
//            throw std::runtime_error("Missing enemy property 'type'");
//          if (auto p = std::any_cast<int>(&propEType->getValue())) {
//            e.type = *p;
//          } else {
//            throw std::runtime_error("Enemy property 'type' is not an int");
//          }
//
//          auto *propVx = obj.getProp("velocityX");
//          if (!propVx)
//            throw std::runtime_error("Missing enemy property 'velocityX'");
//          if (auto p = std::any_cast<int>(&propVx->getValue())) {
//            e.velocity.x = *p;
//          } else {
//            throw std::runtime_error(
//              "Enemy property 'velocityX' is not an int");
//          }
//
//          auto *propVy = obj.getProp("velocityY");
//          if (!propVy)
//            throw std::runtime_error("Missing enemy property 'velocityY'");
//          if (auto p = std::any_cast<int>(&propVy->getValue())) {
//            e.velocity.y = *p;
//          } else {
//            throw std::runtime_error(
//              "Enemy property 'velocityY' is not an int");
//          }
//
//          enemyMapStore.push_back(e);
//        }
//        break;
//      }
//      default:
//        break;
//    }
//  }
//}
