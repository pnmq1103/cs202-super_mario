#include "include/core/resource_manager.hpp"

std::unordered_map<std::string, Texture> ResManager::mario_normal;
std::unordered_map<std::string, Texture> ResManager::mario_star;
std::unordered_map<std::string, Texture> ResManager::mario_fire;
std::unordered_map<std::string, Texture> ResManager::luigi_normal;
std::unordered_map<std::string, Texture> ResManager::luigi_star;
std::unordered_map<std::string, Texture> ResManager::luigi_fire;
std::unordered_map<std::string, Texture> ResManager::luigi_electric;
std::unordered_map<std::string, Texture> ResManager::enemies;
std::unordered_map<std::string, Texture> ResManager::tileset;
std::unordered_map<std::string, Texture> ResManager::icons;
std::unordered_map<std::string, Texture> ResManager::backgrounds;

std::unordered_map<std::string, Music> ResManager::musics;
std::unordered_map<std::string, Sound> ResManager::sounds;

// For safety reasons, call Shutdown once again on object destruction
ResManager::~ResManager() {
  Shutdown();
}

ResManager::ResManager() {
  LoadTextures(
    "res/sprites/characters/mario_normal.png",
    "res/sprites/characters/normal.txt", "n", mario_normal);
  LoadTextures(
    "res/sprites/characters/mario_star.png",
    "res/sprites/characters/star_fire_electric.txt", "s", mario_star);
  LoadTextures(
    "res/sprites/characters/mario_fire.png",
    "res/sprites/characters/star_fire_electric.txt", "f", mario_fire);
  LoadTextures(
    "res/sprites/characters/luigi_normal.png",
    "res/sprites/characters/normal.txt", "n", luigi_normal);
  LoadTextures(
    "res/sprites/characters/luigi_star.png",
    "res/sprites/characters/star_fire_electric.txt", "s", luigi_star);
  LoadTextures(
    "res/sprites/characters/luigi_fire.png",
    "res/sprites/characters/star_fire_electric.txt", "f", luigi_fire);
  LoadTextures(
    "res/sprites/characters/luigi_electric.png",
    "res/sprites/characters/star_fire_electric.txt", "e", luigi_electric);

  LoadTextures(
    "res/sprites/characters/enemies.png", "res/sprites/characters/enemies.txt",
    "enemy_", enemies);

  LoadTextures(
    "res/sprites/tilesets/tileset_ground.png",
    "res/sprites/tilesets/tileset_ground.txt", "tile_", tileset);

  LoadTextures(
    "res/sprites/tilesets/tileset_sky.png",
    "res/sprites/tilesets/tileset_sky.txt", "tile_", tileset);

  LoadTextures(
    "res/sprites/tilesets/tileset_underground.png",
    "res/sprites/tilesets/tileset_underground.txt", "tile_", tileset);

  LoadTextures(
    "res/sprites/tilesets/tileset_underwater.png",
    "res/sprites/tilesets/tileset_underwater.txt", "tile_", tileset);

  LoadTextures(
    "res/sprites/icons/icons.png", "res/sprites/icons/icons.txt", "icon_",
    icons);

  LoadTextures(
    "res/sprites/icons/objects.png", "res/sprites/icons/objects.txt", "icon_",
    icons);

  LoadTextures(
    "res/sprites/backgrounds/backgrounds.png",
    "res/sprites/backgrounds/background.txt", "background_", backgrounds);

  LoadMusic();
  LoadSounds();
}

void ResManager::LoadHelper(
  const fs::path &imgPath, const fs::path &coorPath, std::string key,
  std::unordered_map<std::string, Texture> &map) {

  std::vector<std::pair<Vector2, Vector2>> coors;
  // load img
  std::string path = imgPath.string();
  Image img        = LoadImage(path.c_str());

  // load coors
  std::ifstream in(coorPath);
  // exception handling
  if (!in.is_open())
    throw std::runtime_error("Could not open " + path);
  if (!img.data)
    throw std::runtime_error("Failed to load image: " + imgPath.string());

  std::string line;
  while (std::getline(in, line)) {
    float num, x, y, width, height;
    std::istringstream iss(line);
    if (iss >> num >> x >> y >> width >> height) {
      Vector2 pos  = {x, y};
      Vector2 size = {width, height};
      coors.push_back({pos, size});
    } else {
      std::cerr << "Coordinate input error." << std::endl;
    }
  }
  // fill the texture map
  for (size_t i = 0; i < coors.size(); ++i) {
    std::string tempKey = key + "_" + std::to_string(i + 1);
    Rectangle cropRec   = {
      coors[i].first.x, coors[i].first.y, coors[i].second.x, coors[i].second.y};

    Image cropImg   = ImageFromImage(img, cropRec);
    Texture textImg = LoadTextureFromImage(cropImg);

    map[tempKey] = textImg;

    // cleanup after each loop
    UnloadImage(cropImg);
  }
  // cleanup
  coors.clear();
  UnloadImage(img);
}

void ResManager::LoadTextures(
  const fs::path &imgPath, const fs::path &coorPath, std::string keyPrefix,
  std::unordered_map<std::string, Texture> &textMap) {
  LoadHelper(imgPath, coorPath, keyPrefix, textMap);
}

void ResManager::LoadMusic() {
  musics["bonus"]         = LoadMusicStream(("res/musics/bonus.ogg"));
  musics["boss"]          = LoadMusicStream(("res/musics/boss.ogg"));
  musics["bowser_battle"] = LoadMusicStream(("res/musics/bowser_battle.ogg"));
  musics["castle_theme"]  = LoadMusicStream(("res/musics/castle_theme.ogg"));
  musics["choose_character"]
    = LoadMusicStream(("res/musics/choose_character.ogg"));
  musics["ending"]       = LoadMusicStream(("res/musics/ending.ogg"));
  musics["final_battle"] = LoadMusicStream(("res/musics/final_battle.ogg"));
  musics["ground_theme"] = LoadMusicStream(("res/musics/ground_theme.ogg"));
  musics["invincibility_theme"]
    = LoadMusicStream(("res/musics/invincibility_theme.ogg"));
  musics["overworld"] = LoadMusicStream(("res/musics/overworld.ogg"));
  musics["title"]     = LoadMusicStream(("res/musics/title.ogg"));
  musics["underground_theme"]
    = LoadMusicStream(("res/musics/underground_theme.ogg"));
  musics["underwater_theme"]
    = LoadMusicStream(("res/musics/underwater_theme.ogg"));
}

void ResManager::LoadSounds() {
  sounds["1up"]             = LoadSound(("res/sounds/1up.wav"));
  sounds["beep"]            = LoadSound(("res/sounds/beep.wav"));
  sounds["billfirework"]    = LoadSound(("res/sounds/billfirework.wav"));
  sounds["bowserfall"]      = LoadSound(("res/sounds/bowserfall.wav"));
  sounds["brick"]           = LoadSound(("res/sounds/brick.wav"));
  sounds["bump"]            = LoadSound(("res/sounds/bump.wav"));
  sounds["castle_complete"] = LoadSound(("res/sounds/castle_complete.wav"));
  sounds["coin"]            = LoadSound(("res/sounds/coin.wav"));
  sounds["course_clear"]    = LoadSound(("res/sounds/course_clear.wav"));
  sounds["fire"]            = LoadSound(("res/sounds/fire.wav"));
  sounds["fireball"]        = LoadSound(("res/sounds/fireball.wav"));
  sounds["flagpole"]        = LoadSound(("res/sounds/flagpole.wav"));
  sounds["gameover"]        = LoadSound(("res/sounds/gameover.wav"));
  sounds["gameover_unused"] = LoadSound(("res/sounds/gameover_unused.wav"));
  sounds["hurryup"]         = LoadSound(("res/sounds/hurryup.wav"));
  sounds["item"]            = LoadSound(("res/sounds/item.wav"));
  sounds["jump"]            = LoadSound(("res/sounds/jump.wav"));
  sounds["jumpsmall"]       = LoadSound(("res/sounds/jumpsmall.wav"));
  sounds["kickkill"]        = LoadSound(("res/sounds/kickkill.wav"));
  sounds["level_complete"]  = LoadSound(("res/sounds/level_complete.wav"));
  sounds["life_lost"]       = LoadSound(("res/sounds/life_lost.wav"));
  sounds["pause"]           = LoadSound(("res/sounds/pause.wav"));
  sounds["pipepowerdown"]   = LoadSound(("res/sounds/pipepowerdown.wav"));
  sounds["powerup"]         = LoadSound(("res/sounds/powerup.wav"));
  sounds["stompswim"]       = LoadSound(("res/sounds/stompswim.wav"));
  sounds["time-up_warning"] = LoadSound(("res/sounds/time-up_warning.wav"));
  sounds["vine"]            = LoadSound(("res/sounds/vine.wav"));
  sounds["world_clear"]     = LoadSound(("res/sounds/world_clear.wav"));
}

SaveData ResManager::LoadResourcesFromFile() {
  FileHandler fp;
  std::string path = fp.OpenFilePath();
  if (path.empty()) {
    return {};
  }
  SaveData data;
  if (!fp.LoadFile(path, data)) {
    throw std::runtime_error("Failed to load file at: " + path);
  }
  return data;
}

bool ResManager::SaveResourcesToFile(const SaveData &data) {
  FileHandler fp;
  std::string path = fp.OpenSavePath();
  if (path.empty()) {
    return false;
  }
  return fp.SaveFile(path, data);
}

Texture ResManager::GetMario(char type, int index) {
  std::string key;
  switch (type) {
    case 'n': {
      std::string key = "n" + std::to_string(index);
      auto it         = mario_normal.find(key);
      if (it == mario_normal.end())
        throw std::out_of_range("Missing character: " + key);
      return it->second;
    } break;
    case 's': {
      std::string key = "s" + std::to_string(index);
      auto it         = mario_star.find(key);
      if (it == mario_star.end())
        throw std::out_of_range("Missing character: " + key);
      return it->second;
    } break;
    case 'f': {
      std::string key = "f" + std::to_string(index);
      auto it         = mario_fire.find(key);
      if (it == mario_fire.end())
        throw std::out_of_range("Missing character: " + key);
      return it->second;
    } break;
    default:
      throw std::runtime_error("Invalid type");
  }
}

Texture ResManager::GetLuigi(char type, int index) {
  std::string key;
  switch (type) {
    case 'n': {
      std::string key = "n" + std::to_string(index);
      auto it         = luigi_normal.find(key);
      if (it == luigi_normal.end())
        throw std::out_of_range("Missing character: " + key);
      return it->second;
    } break;
    case 's': {
      std::string key = "s" + std::to_string(index);
      auto it         = luigi_star.find(key);
      if (it == luigi_star.end())
        throw std::out_of_range("Missing character: " + key);
      return it->second;
    } break;
    case 'f': {
      std::string key = "f" + std::to_string(index);
      auto it         = luigi_fire.find(key);
      if (it == luigi_fire.end())
        throw std::out_of_range("Missing character: " + key);
      return it->second;
    } break;
    case 'e': {
      std::string key = "e" + std::to_string(index);
      auto it         = luigi_electric.find(key);
      if (it == luigi_electric.end())
        throw std::out_of_range("Missing character: " + key);
      return it->second;
    } break;
    default:
      throw std::runtime_error("Invalid type");
  }
}

Texture ResManager::GetEnemy(int index) {
  std::string key = "enemy_" + std::to_string(index);
  auto it         = enemies.find(key);
  if (it == enemies.end())
    throw std::out_of_range("Missing enemy: " + key);
  return it->second;
}

Texture ResManager::GetTile(int index) {
  std::string key = "tile_" + std::to_string(index);
  auto it         = tileset.find(key);
  if (it == tileset.end())
    throw std::out_of_range("Missing tile: " + key);
  return it->second;
}

Texture ResManager::GetIcon(int index) {
  std::string key = "icon_" + std::to_string(index);
  auto it         = icons.find(key);
  if (it == icons.end())
    throw std::out_of_range("Missing icon: " + key);
  return it->second;
}

Texture ResManager::GetBackground(int index) {
  std::string key = "background_" + std::to_string(index);
  auto it         = backgrounds.find(key);
  if (it == backgrounds.end())
    throw std::out_of_range("Missing background: " + key);
  return it->second;
}

Music ResManager::GetMusic(std::string key) {
  auto it = musics.find(key);
  if (it == musics.end())
    throw std::out_of_range("Missing music: " + key);
  return it->second;
}

Sound ResManager::GetSound(std::string key) {
  auto it = sounds.find(key);
  if (it == sounds.end())
    throw std::out_of_range("Missing sound: " + key);
  return it->second;
}

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