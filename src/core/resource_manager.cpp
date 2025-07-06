#include "..\include\core\resource_manager.hpp"

std::map<std::string, Texture2D> ResManager::characters;
std::map<std::string, Texture2D> ResManager::blocks;
std::map<std::string, Texture2D> ResManager::backgrounds;
std::map<std::string, Music> ResManager::musics;
std::map<std::string, Sound> ResManager::sounds;

ResManager::ResManager() {
  loadCharacters();
  loadBlocks();
  loadBackgrounds();
  loadMusic();
  loadSounds();
}

void ResManager::loadTexture(
  const fs::path& imgPath, const fs::path& coorPath, std::string key) {
  // load img
  std::string path = imgPath.string();
  Image img = LoadImage(path.c_str());

  // load coors
  std::ifstream in(coorPath);

  if (!in.is_open())
    throw std::runtime_error("Could not open " + path);

  std::string line;
  while (std::getline(in, line)) {
    float num, x, y, width, height;
    std::istringstream iss(line);
    if (iss >> num >> x >> y >> width >> height) {
      Vector2 pos  = {x, y};
      Vector2 size = {width, height};
      coors.push_back({pos, size});
    } else {
      std::cerr << "Coor input error." << std::endl;
    }
  }
  // fill the texture map
  for (size_t i = 0; i < coors.size(); ++i) {
    std::string tempKey = key + "_" + std::to_string(i + 1);
    Rectangle cropRec   = {
      coors[i].first.x, coors[i].first.y, coors[i].second.x, coors[i].second.y};

    Image cropImg     = ImageFromImage(img, cropRec);
    Texture2D textImg = LoadTextureFromImage(cropImg);

    tempText[tempKey] = textImg;

    // cleanup after each loop
    UnloadImage(cropImg);
  }
  // cleanup
  coors.clear();
  UnloadImage(img);
}

#include <filesystem>
namespace fs = std::filesystem;

void ResManager::loadCharacters() {
  fs::path tmpImgDir  = imgPath / "characters";
  fs::path tmpCoorDir = coorPath / "characters";

  loadTexture(
    (tmpImgDir / "bowser.png").string(), (tmpCoorDir / "bowser.txt").string(),
    "bowser");

  loadTexture(
    (tmpImgDir / "enemies.png").string(), (tmpCoorDir / "enemies.txt").string(),
    "enemies");

  loadTexture(
    (tmpImgDir / "luigi_normal.png").string(),
    (tmpCoorDir / "luigi_normal.txt").string(), "luigi_normal");

  loadTexture(
    (tmpImgDir / "luigi_normal_carry_shoe_shell.png").string(),
    (tmpCoorDir / "luigi_normal_carry_shoe_shell.txt").string(),
    "luigi_normal_carry_shoe_shell");

  loadTexture(
    (tmpImgDir / "luigi_starup.png").string(),
    (tmpCoorDir / "luigi_starup.txt").string(), "luigi_starup");

  loadTexture(
    (tmpImgDir / "luigi_starup_carry_shoe_shell.png").string(),
    (tmpCoorDir / "luigi_starup_carry_shoe_shell.txt").string(),
    "luigi_starup_carry_shoe_shell");

  loadTexture(
    (tmpImgDir / "luigi_starup_carry_shoe_shell_fire.png").string(),
    (tmpCoorDir / "luigi_starup_carry_shoe_shell_fire.txt").string(),
    "luigi_starup_carry_shoe_shell_fire");

  loadTexture(
    (tmpImgDir / "luigi_starup_fire.png").string(),
    (tmpCoorDir / "luigi_starup_fire.txt").string(), "luigi_starup_fire");

  loadTexture(
    (tmpImgDir / "mario_normal.png").string(),
    (tmpCoorDir / "mario_normal.txt").string(), "mario_normal");

  loadTexture(
    (tmpImgDir / "mario_normal_carry_shoe_shell.png").string(),
    (tmpCoorDir / "mario_normal_carry_shoe_shell.txt").string(),
    "mario_normal_carry_shoe_shell");

  loadTexture(
    (tmpImgDir / "mario_starup.png").string(),
    (tmpCoorDir / "mario_starup.txt").string(), "mario_starup");

  loadTexture(
    (tmpImgDir / "mario_starup_carry_shoe_shell.png").string(),
    (tmpCoorDir / "mario_starup_carry_shoe_shell.txt").string(),
    "mario_starup_carry_shoe_shell");

  loadTexture(
    (tmpImgDir / "mario_starup_carry_shoe_shell_fire.png").string(),
    (tmpCoorDir / "mario_starup_carry_shoe_shell_fire.txt").string(),
    "mario_starup_carry_shoe_shell_fire");

  loadTexture(
    (tmpImgDir / "mario_starup_fire.png").string(),
    (tmpCoorDir / "mario_starup_fire.txt").string(), "mario_starup_fire");

  characters.merge(tempText);
  tempText.clear();
}

void ResManager::loadBlocks() {
  fs::path tmpImgDir  = imgPath / "icons";
  fs::path tmpCoorDir = coorPath / "icons";

  loadTexture(
    (tmpImgDir / "icons.png").string(), (tmpCoorDir / "icons.txt").string(),
    "icon");

  loadTexture(
    (tmpImgDir / "objects.png").string(), (tmpCoorDir / "objects.txt").string(),
    "objects");

  tmpImgDir  = imgPath / "tileset";
  tmpCoorDir = coorPath / "tileset";

  loadTexture(
    (tmpImgDir / "tileset_ground.png").string(),
    (tmpCoorDir / "tileset_ground.txt").string(), "tileset_ground");

  loadTexture(
    (tmpImgDir / "tileset_sky.png").string(),
    (tmpCoorDir / "tileset_sky.txt").string(), "tileset_sky");

  loadTexture(
    (tmpImgDir / "tileset_underground.png").string(),
    (tmpCoorDir / "tileset_underground.txt").string(), "tileset_underground");

  loadTexture(
    (tmpImgDir / "tileset_underwater.png").string(),
    (tmpCoorDir / "tileset_underwater.txt").string(), "tileset_underwater");

  tmpImgDir  = imgPath / "user_interface";
  tmpCoorDir = coorPath / "user_interface";

  loadTexture(
    (tmpImgDir / "buttons.png").string(), (tmpCoorDir / "buttons.txt").string(),
    "UI");

  blocks.merge(tempText);
  tempText.clear();
}

void ResManager::loadBackgrounds() {
  fs::path tmpImgDir  = imgPath / "backgrounds";
  fs::path tmpCoorDir = coorPath / "backgrounds";

  loadTexture(tmpImgDir.string(), tmpCoorDir.string(), "backgrounds");

  backgrounds.merge(tempText);
  tempText.clear();
}


void ResManager::loadMusic() {
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

void ResManager::loadSounds() {
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


SaveDatawMap ResManager::loadResourcesFromFile() {
  FileHandler fp;
  std::string path = fp.openFilePath();
  if (path.empty()) {
    return {};
  }
  SaveDatawMap data;
  if (!fp.loadFile(path, data)) {
    throw std::runtime_error(
      "Failed to load file at: " + path);
  }
  return data;
}

bool ResManager::saveResourcesToFile(const SaveDatawMap &data) {
  FileHandler fp;
  std::string path = fp.openSavePath(); 
  if (path.empty()) {
    return false;
  }
  return fp.saveFile(path, data);
}

Texture2D ResManager::getCharacter(std::string key) {
  auto it = characters.find(key);
  if (it == characters.end())
    throw std::out_of_range("Missing character: " + key);
  return it->second;
}

Texture2D ResManager::getBlock(std::string key) {
  auto it = blocks.find(key);
  if (it == blocks.end())
    throw std::out_of_range("Missing block: " + key);
  return it->second;
}

Texture2D ResManager::getBackground(int index) {
  std::string key = "backgrounds_" + std::to_string(index);
  auto it         = backgrounds.find(key);
  if (it == backgrounds.end())
    throw std::out_of_range("Missing background: " + key);
  return it->second;
}

Music ResManager::getMusic(std::string key) {
  auto it = musics.find(key);
  if (it == musics.end())
    throw std::out_of_range("Missing music: " + key);
  return it->second;
}

Sound ResManager::getSound(std::string key) {
  auto it = sounds.find(key);
  if (it == sounds.end())
    throw std::out_of_range("Missing sound: " + key);
  return it->second;
}

ResManager::~ResManager() {
  for (auto &p : characters) {
    UnloadTexture(p.second);
  }
  characters.clear();
  for (auto &p : blocks) {
    UnloadTexture(p.second);
  }
  blocks.clear();

  for (auto &p : backgrounds) {
    UnloadTexture(p.second);
  }
  backgrounds.clear();

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