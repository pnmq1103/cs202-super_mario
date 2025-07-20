#include <array>
#include <fstream>

#include "include/core/resource_manager.hpp"

ResManager::~ResManager() {
  auto Unload = [&](auto &mp) {
    for (auto &p : mp)
      UnloadTexture(p.second);
    mp.clear();
  };
  Unload(mario_normal);
  Unload(mario_star);
  Unload(mario_fire);
  Unload(luigi_normal);
  Unload(luigi_star);
  Unload(luigi_fire);
  Unload(luigi_electric);
  Unload(enemies);
  Unload(icons);
  Unload(tileset);
  Unload(backgrounds);

  for (auto &sound : sounds)
    UnloadSound(sound.second);
  sounds.clear();

  for (auto &music : musics) {
    StopMusicStream(music.second);
    UnloadMusicStream(music.second);
  }
  musics.clear();
}

ResManager::ResManager() {}

void ResManager::Init() {
  LoadTextures();
  LoadMusic();
  LoadSounds();
}

void ResManager::LoadHelper(
  const fs::path &img_path, std::string key,
  std::unordered_map<std::string, Texture> &texture_map) {

  // Load image
  std::string path = img_path.string();
  Image img        = LoadImage(path.c_str());

  // Load info
  fs::path info_path = img_path;
  info_path.replace_extension(".txt");
  std::ifstream fin(info_path);

  if (!fin.is_open())
    throw std::runtime_error("Could not open file");
  if (!img.data)
    throw std::runtime_error("Failed to load image");

  std::vector<Rectangle> recs;
  int id;
  float x, y, width, height;
  while (fin >> id) {
    if (fin >> x >> y >> width >> height)
      recs.push_back({x, y, width, height});
    else
      throw std::runtime_error("malformed input");
  }

  // Fill the texture map
  for (size_t i = 0; i < recs.size(); ++i) {
    std::string tmp_key = key + "_" + std::to_string(i + 1);
    Rectangle cropRec   = {recs[i].x, recs[i].y, recs[i].width, recs[i].height};

    Image cropped_img    = ImageFromImage(img, cropRec);
    Texture cropped_txtr = LoadTextureFromImage(cropped_img);

    texture_map[tmp_key] = cropped_txtr;

    // Cleanup after each loop
    UnloadImage(cropped_img);
  }

  recs.clear();
  UnloadImage(img);
}

void ResManager::LoadTextures() {
  LoadHelper("res/sprites/characters/mario_normal.png", "n", mario_normal);
  LoadHelper("res/sprites/characters/mario_star.png", "s", mario_star);
  LoadHelper("res/sprites/characters/mario_fire.png", "f", mario_fire);

  LoadHelper("res/sprites/characters/luigi_normal.png", "n", luigi_normal);
  LoadHelper("res/sprites/characters/luigi_star.png", "s", luigi_star);
  LoadHelper("res/sprites/characters/luigi_fire.png", "f", luigi_fire);
  LoadHelper("res/sprites/characters/luigi_electric.png", "e", luigi_electric);

  LoadHelper("res/sprites/characters/enemies.png", "enemy_", enemies);

  LoadHelper("res/sprites/tilesets/tileset_ground.png", "tile_", tileset);
  LoadHelper("res/sprites/tilesets/tileset_sky.png", "tile_", tileset);
  LoadHelper("res/sprites/tilesets/tileset_underground.png", "tile_", tileset);
  LoadHelper("res/sprites/tilesets/tileset_underwater.png", "tile_", tileset);

  LoadHelper("res/sprites/icons/icons.png", "icon_", icons);
  LoadHelper("res/sprites/icons/objects.png", "icon_", icons);

  LoadHelper(
    "res/sprites/backgrounds/backgrounds.png", "background_", backgrounds);
}

void ResManager::LoadMusic() {
  auto Load = [this](const std::string &name) {
    musics[name] = LoadMusicStream(("res/music/" + name + ".ogg").c_str());
  };

  std::array<std::string, 13> names = {
    "bonus",
    "boss",
    "bowser_battle",
    "castle_theme",
    "choose_character",
    "ending",
    "final_battle",
    "ground_theme",
    "invincibility_theme",
    "overworld",
    "title",
    "underground_theme",
    "underwater_theme"};

  for (const auto &name : names)
    Load(name);
}

void ResManager::LoadSounds() {
  auto Load = [this](const std::string &name) {
    sounds[name] = LoadSound(("res/sounds/" + name + ".wav").c_str());
  };

  std::array<std::string, 28> names = {
    "1up",
    "beep",
    "billfirework",
    "bowserfall",
    "brick",
    "bump",
    "castle_complete",
    "coin",
    "course_clear",
    "fire",
    "fireball",
    "flagpole",
    "gameover",
    "gameover_unused",
    "hurryup",
    "item",
    "jump",
    "jumpsmall",
    "kickkill",
    "level_complete",
    "life_lost",
    "pause",
    "pipepowerdown",
    "powerup",
    "stompswim",
    "time-up_warning",
    "vine",
    "world_clear",
  };

  for (const auto &name : names)
    Load(name);
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

Texture ResManager::GetMario(char type, int idx) {
  std::string key = std::string(1, type) + std::to_string(idx);
  std::unordered_map<std::string, Texture> *form;
  switch (type) {
    case 'n':
      form = &mario_normal;
      break;
    case 's':
      form = &mario_star;
      break;
    case 'f':
      form = &mario_fire;
      break;
    default:
      throw std::runtime_error("Invalid Mario type");
  }

  auto it = form->find(key);
  if (it == form->end())
    throw std::out_of_range("Missing character");
  return it->second;
}

Texture ResManager::GetLuigi(char type, int idx) {
  std::string key = std::string(1, type) + std::to_string(idx);
  std::unordered_map<std::string, Texture> *form = nullptr;
  switch (type) {
    case 'n':
      form = &luigi_normal;
      break;
    case 's':
      form = &luigi_star;
      break;
    case 'f':
      form = &luigi_fire;
      break;
    case 'e':
      form = &luigi_electric;
      break;
    default:
      throw std::runtime_error("Invalid Luigi type");
  }

  auto it = form->find(key);
  if (it == form->end())
    throw std::out_of_range("Missing character");
  return it->second;
}

Texture ResManager::GetEnemy(int idx) {
  std::string key = "enemy_" + std::to_string(idx);
  auto it         = enemies.find(key);
  if (it == enemies.end())
    throw std::out_of_range("Missing enemy");
  return it->second;
}

Texture ResManager::GetTile(int idx) {
  std::string key = "tile_" + std::to_string(idx);
  auto it         = tileset.find(key);
  if (it == tileset.end())
    throw std::out_of_range("Missing tile");
  return it->second;
}

Texture ResManager::GetIcon(int idx) {
  std::string key = "icon_" + std::to_string(idx);
  auto it         = icons.find(key);
  if (it == icons.end())
    throw std::out_of_range("Missing icon");
  return it->second;
}

Texture ResManager::GetBackground(int idx) {
  std::string key = "background_" + std::to_string(idx);
  auto it         = backgrounds.find(key);
  if (it == backgrounds.end())
    throw std::out_of_range("Missing background");
  return it->second;
}

Music ResManager::GetMusic(std::string key) {
  auto it = musics.find(key);
  if (it == musics.end())
    throw std::out_of_range("Missing music");
  return it->second;
}

Sound ResManager::GetSound(std::string key) {
  auto it = sounds.find(key);
  if (it == sounds.end())
    throw std::out_of_range("Missing sound");
  return it->second;
}

/*
// call this function before exiting game to clean up everything
void ResManager::Shutdown() {
  // lambda for unloading unordered_maps
  auto unloadAll = [&](auto &mp) {
    for (auto &p : mp)
      UnloadTexture(p.second);
    mp.clear();
  };
  unloadAll(mario_normal);
  unloadAll(mario_star);
  unloadAll(mario_fire);
  unloadAll(luigi_normal);
  unloadAll(luigi_star);
  unloadAll(luigi_fire);
  unloadAll(luigi_electric);
  unloadAll(enemies);
  unloadAll(icons);
  unloadAll(tileset);
  unloadAll(backgrounds);

  for (auto &p : sounds) {
    UnloadSound(p.second);
  }
  sounds.clear();
  for (auto &p : musics) {
    StopMusicStream(p.second);
    UnloadMusicStream(p.second);
  }
  musics.clear();
}
*/
bool ResManager::LoadMap(const std::string &path, std::vector<Block*> &blockData) {
  tson::Tileson t;
  std::unique_ptr<tson::Map> map = t.parse(path);
  if (map->getStatus() == tson::ParseStatus::OK) {
    // get map dimension
    int mWidth  = map->getTileSize().x;
    int mHeight = map->getTileSize().y;
    // for when more than one tileset is included in the map
    struct TilesetInfo {
      int firstGid, columns, margin, spacing;
      Vector2 tileSize;
    };
    std::vector<TilesetInfo> tsi;
    // get all tilesets for convenience
    for (auto &ts : map->getTilesets()) {
      std::string texPath = ts.getImagePath().string();
      Texture tex         = LoadTexture(texPath.c_str());

      int margin          = ts.getMargin();
      int spacing         = ts.getSpacing();
      int tileW           = ts.getTileSize().x;
      int tileH           = ts.getTileSize().y;
      int cols = (tex.width - 2 * margin + spacing) / (tileW + spacing);
      //mark each texture with its first Gid
      tilesetMapStore[ts.getFirstgid()] = tex;
      tsi.push_back(
        {ts.getFirstgid(),
         cols,
         margin,
         spacing,
         {float(tileW), float(tileH)}});
    }
    // sort the tileset according to Gid
    std::sort(tsi.begin(), tsi.end(), [](auto &a, auto &b) {
      return a.firstGid < b.firstGid;
    });
    // loop over layers
    for (auto &layer : map->getLayers()) {
      switch (layer.getType()) {
        case tson::LayerType::TileLayer: {
          if (!map->isInfinite()) {
            std::map<std::tuple<int, int>, tson::Tile *> tData
              = layer.getTileData();
            for (const auto &[id, tile] : tData) {
              Block* b;
              b->SetSpriteId(tile->getGid());
              b->SetSize(tile->getTileSize().x, tile->getTileSize().y);
              b->SetPosition({tile->getPosition(id).x, tile->getPosition(id).y});
              b->SetType(tile->getType());
              blockData.push_back(b);
            }
          }
        }
      }
    }
  }
