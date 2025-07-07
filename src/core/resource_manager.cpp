#include "..\include\core\resource_manager.hpp"

std::map<std::string, Texture2D> ResManager::mario_normal;
std::map<std::string, Texture2D> ResManager::mario_star;
std::map<std::string, Texture2D> ResManager::mario_fire;
std::map<std::string, Texture2D> ResManager::luigi_normal;
std::map<std::string, Texture2D> ResManager::luigi_star;
std::map<std::string, Texture2D> ResManager::luigi_fire;
std::map<std::string, Texture2D> ResManager::luigi_electric;
std::map<std::string, Texture2D> ResManager::enemies;
std::map<std::string, Texture2D> ResManager::tileset;
std::map<std::string, Texture2D> ResManager::icons;
std::map<std::string, Texture2D> ResManager::backgrounds;

std::map<std::string, Music> ResManager::musics;
std::map<std::string, Sound> ResManager::sounds;

ResManager::ResManager() {
  LoadTexture(
    "res/sprites/characters/mario_normal.png",
    "res/sprites/characters/normal.txt", "n", mario_normal);
  LoadTexture(
    "res/sprites/characters/mario_star.png",
    "res/sprites/characters/star_fire_electric.txt", "s", mario_star);
  LoadTexture(
    "res/sprites/characters/mario_fire.png",
    "res/sprites/characters/star_fire_electric.txt", "f", mario_fire);
  LoadTexture(
    "res/sprites/characters/luigi_normal.png",
    "res/sprites/characters/normal.txt", "n", luigi_normal);
  LoadTexture(
    "res/sprites/characters/luigi_star.png",
    "res/sprites/characters/star_fire_electric.txt", "s", luigi_star);
  LoadTexture(
    "res/sprites/characters/luigi_fire.png",
    "res/sprites/characters/star_fire_electric.txt", "f", luigi_fire);
  LoadTexture(
    "res/sprites/characters/luigi_electric.png",
    "res/sprites/characters/star_fire_electric.txt", "e", luigi_electric);

  LoadTexture(
    "res/sprites/characters/enemies.png", "res/sprites/characters/enemies.txt",
    "enemy_", enemies);

  LoadTexture(
    "res/sprites/tilesets/tileset_ground.png",
    "res/sprites/tilesets/tileset_ground.txt", "tile_", tileset);

  LoadTexture(
    "res/sprites/tilesets/tileset_sky.png",
    "res/sprites/tilesets/tileset_sky.txt", "tile_", tileset);

  LoadTexture(
    "res/sprites/tilesets/tileset_underground.png",
    "res/sprites/tilesets/tileset_underground.txt", "tile_", tileset);

  LoadTexture(
    "res/sprites/tilesets/tileset_underwater.png",
    "res/sprites/tilesets/tileset_underwater.txt", "tile_", tileset);

  LoadTexture(
    "res/sprites/icons/icons.png", "res/sprites/icons/icons.txt", "icon_",
    icons);

  LoadTexture(
    "res/sprites/icons/objects.png", "res/sprites/icons/objects.txt", "icon_",
    icons);

  LoadTexture(
    "res/sprites/backgrounds/backgrounds.png",
    "res/sprites/backgrounds/background.txt", "background_", backgrounds);

  LoadMusic();
  LoadSounds();
}

void ResManager::LoadHelper(
  const fs::path &imgPath, const fs::path &coorPath, std::string key,
  std::map<std::string, Texture2D> &map) {

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

    Image cropImg     = ImageFromImage(img, cropRec);
    Texture2D textImg = LoadTextureFromImage(cropImg);

    map[tempKey] = textImg;

    // cleanup after each loop
    UnloadImage(cropImg);
  }
  // cleanup
  coors.clear();
  UnloadImage(img);
}

void ResManager::LoadTexture(
  const fs::path &imgPath, const fs::path &coorPath, std::string keyPrefix,
  std::map<std::string, Texture2D> &textMap) {
  LoadHelper(imgPath, coorPath, keyPrefix, textMap);
}

void ResManager::LoadMusic() {
  musics["bonus"] = LoadMusicStream((musicPath / "bonus.ogg").string().c_str());
  musics["boss"]  = LoadMusicStream((musicPath / "boss.ogg").string().c_str());
  musics["bowser_battle"]
    = LoadMusicStream((musicPath / "bowser_battle.ogg").string().c_str());
  musics["castle_theme"]
    = LoadMusicStream((musicPath / "castle_theme.ogg").string().c_str());
  musics["choose_character"]
    = LoadMusicStream((musicPath / "choose_character.ogg").string().c_str());
  musics["ending"]
    = LoadMusicStream((musicPath / "ending.ogg").string().c_str());
  musics["final_battle"]
    = LoadMusicStream((musicPath / "final_battle.ogg").string().c_str());
  musics["ground_theme"]
    = LoadMusicStream((musicPath / "ground_theme.ogg").string().c_str());
  musics["invincibility_theme"]
    = LoadMusicStream((musicPath / "invincibility_theme.ogg").string().c_str());
  musics["overworld"]
    = LoadMusicStream((musicPath / "overworld.ogg").string().c_str());
  musics["title"] = LoadMusicStream((musicPath / "title.ogg").string().c_str());
  musics["underground_theme"]
    = LoadMusicStream((musicPath / "underground_theme.ogg").string().c_str());
  musics["underwater_theme"]
    = LoadMusicStream((musicPath / "underwater_theme.ogg").string().c_str());
}

void ResManager::LoadSounds() {
  sounds["1up"]  = LoadSound((soundPath / "1up.wav").string().c_str());
  sounds["beep"] = LoadSound((soundPath / "beep.wav").string().c_str());
  sounds["billfirework"]
    = LoadSound((soundPath / "billfirework.wav").string().c_str());
  sounds["bowserfall"]
    = LoadSound((soundPath / "bowserfall.wav").string().c_str());
  sounds["brick"] = LoadSound((soundPath / "brick.wav").string().c_str());
  sounds["bump"]  = LoadSound((soundPath / "bump.wav").string().c_str());
  sounds["castle_complete"]
    = LoadSound((soundPath / "castle_complete.wav").string().c_str());
  sounds["coin"] = LoadSound((soundPath / "coin.wav").string().c_str());
  sounds["course_clear"]
    = LoadSound((soundPath / "course_clear.wav").string().c_str());
  sounds["fire"]     = LoadSound((soundPath / "fire.wav").string().c_str());
  sounds["fireball"] = LoadSound((soundPath / "fireball.wav").string().c_str());
  sounds["flagpole"] = LoadSound((soundPath / "flagpole.wav").string().c_str());
  sounds["gameover"] = LoadSound((soundPath / "gameover.wav").string().c_str());
  sounds["gameover_unused"]
    = LoadSound((soundPath / "gameover_unused.wav").string().c_str());
  sounds["hurryup"] = LoadSound((soundPath / "hurryup.wav").string().c_str());
  sounds["item"]    = LoadSound((soundPath / "item.wav").string().c_str());
  sounds["jump"]    = LoadSound((soundPath / "jump.wav").string().c_str());
  sounds["jumpsmall"]
    = LoadSound((soundPath / "jumpsmall.wav").string().c_str());
  sounds["kickkill"] = LoadSound((soundPath / "kickkill.wav").string().c_str());
  sounds["level_complete"]
    = LoadSound((soundPath / "level_complete.wav").string().c_str());
  sounds["life_lost"]
    = LoadSound((soundPath / "life_lost.wav").string().c_str());
  sounds["pause"] = LoadSound((soundPath / "pause.wav").string().c_str());
  sounds["pipepowerdown"]
    = LoadSound((soundPath / "pipepowerdown.wav").string().c_str());
  sounds["powerup"] = LoadSound((soundPath / "powerup.wav").string().c_str());
  sounds["stompswim"]
    = LoadSound((soundPath / "stompswim.wav").string().c_str());
  sounds["time-up_warning"]
    = LoadSound((soundPath / "time-up_warning.wav").string().c_str());
  sounds["vine"] = LoadSound((soundPath / "vine.wav").string().c_str());
  sounds["world_clear"]
    = LoadSound((soundPath / "world_clear.wav").string().c_str());
}

SaveDatawMap ResManager::LoadResourcesFromFile() {
  FileHandler fp;
  std::string path = fp.OpenFilePath();
  if (path.empty()) {
    return {};
  }
  SaveDatawMap data;
  if (!fp.LoadFile(path, data)) {
    throw std::runtime_error("Failed to load file at: " + path);
  }
  return data;
}

bool ResManager::SaveResourcesToFile(const SaveDatawMap &data) {
  FileHandler fp;
  std::string path = fp.OpenSavePath();
  if (path.empty()) {
    return false;
  }
  return fp.SaveFile(path, data);
}

Texture2D ResManager::GetMario(char type, int index) {
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

Texture2D ResManager::GetLuigi(char type, int index) {
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

Texture2D ResManager::GetEnemy(int index) {
  std::string key = "enemy_" + std::to_string(index);
  auto it         = enemies.find(key);
  if (it == enemies.end())
    throw std::out_of_range("Missing enemy: " + key);
  return it->second;
}

Texture2D ResManager::GetTile(int index) {
  std::string key = "tile_" + std::to_string(index);
  auto it         = tileset.find(key);
  if (it == tileset.end())
    throw std::out_of_range("Missing tile: " + key);
  return it->second;
}

Texture2D ResManager::GetIcon(int index) {
  std::string key = "icon_" + std::to_string(index);
  auto it         = icons.find(key);
  if (it == icons.end())
    throw std::out_of_range("Missing icon: " + key);
  return it->second;
}

Texture2D ResManager::GetBackground(int index) {
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
  // lambda for unloading maps
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

// for safety reasons, call Shutdown once again on object destruction
ResManager::~ResManager() {
  Shutdown();
}