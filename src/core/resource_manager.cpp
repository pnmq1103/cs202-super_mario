#include "..\include\core\resource_manager.hpp"

std::map<std::string, Texture2D> ResManager::characters;
std::map<std::string, Texture2D> ResManager::blocks;
std::map<std::string, Music> ResManager::musics;
std::map<std::string, Sound> ResManager::sounds;

ResManager::ResManager() {
  loadCharacters();
  loadBlocks();
  loadMusic();
  loadSounds();
}

void ResManager::loadTexture(
  std::string imgPath, std::string coorPath, std::string key) {
  // load img
  Image img = LoadImage(imgPath.c_str());

  // load coors
  std::ifstream in(coorPath);

  if (!in.is_open())
    throw std::runtime_error("Could not open " + coorPath);

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

void ResManager::loadCharacters() {
  std::string tmpImgPath  = imgPath + "\\characters\\";
  std::string tmpCoorPath = coorPath + "\\characters\\";

  loadTexture(tmpImgPath + "bowser.png", tmpCoorPath + "bowser.txt", "bowser");

  loadTexture(
    tmpImgPath + "enemies.png", tmpCoorPath + "enemies.txt", "enemies");

  loadTexture(
    tmpImgPath + "luigi_normal.png", tmpCoorPath + "luigi_normal.txt",
    "luigi_normal");

  loadTexture(
    tmpImgPath + "luigi_normal_carry_shoe_shell.png",
    tmpCoorPath + "luigi_normal_carry_shoe_shell.txt",
    "luigi_normal_carry_shoe_shell");

  loadTexture(
    tmpImgPath + "luigi_starup.png", tmpCoorPath + "luigi_starup.txt",
    "luigi_starup");

  loadTexture(
    tmpImgPath + "luigi_starup_carry_shoe_shell.png",
    tmpCoorPath + "luigi_starup_carry_shoe_shell.txt",
    "luigi_starup_carry_shoe_shell");

  loadTexture(
    tmpImgPath + "luigi_starup_carry_shoe_shell_fire.png",
    tmpCoorPath + "luigi_starup_carry_shoe_shell_fire.txt",
    "luigi_starup_carry_shoe_shell_fire");

  loadTexture(
    tmpImgPath + "luigi_starup_fire.png", tmpCoorPath + "luigi_starup_fire.txt",
    "luigi_starup_fire");

  loadTexture(
    tmpImgPath + "mario_normal.png", tmpCoorPath + "mario_normal.txt",
    "mario_normal");

  loadTexture(
    tmpImgPath + "mario_normal_carry_shoe_shell.png",
    tmpCoorPath + "mario_normal_carry_shoe_shell.txt",
    "mario_normal_carry_shoe_shell");

  loadTexture(
    tmpImgPath + "mario_starup.png", tmpCoorPath + "mario_starup.txt",
    "mario_starup");

  loadTexture(
    tmpImgPath + "mario_starup_carry_shoe_shell.png",
    tmpCoorPath + "mario_starup_carry_shoe_shell.txt",
    "mario_starup_carry_shoe_shell");

  loadTexture(
    tmpImgPath + "mario_starup_carry_shoe_shell_fire.png",
    tmpCoorPath + "mario_starup_carry_shoe_shell_fire.txt",
    "mario_starup_carry_shoe_shell_fire");

  loadTexture(
    tmpImgPath + "mario_starup_fire.png", tmpCoorPath + "mario_starup_fire.txt",
    "mario_starup_fire");

  characters.merge(tempText);
  tempText.clear();
}

void ResManager::loadBlocks() {
  std::string tmpImgPath  = imgPath + "\\icons\\";
  std::string tmpCoorPath = coorPath + "\\icons\\";
  loadTexture(
    tmpImgPath + "icon_day.png", tmpCoorPath + "icon_day.txt", "icon_day");

  loadTexture(
    tmpImgPath + "icon_sky.png", tmpCoorPath + "icon_sky.txt", "icon_sky");

  loadTexture(
    tmpImgPath + "icon_underground.png", tmpCoorPath + "icon_underground.txt",
    "icon_underground");

  loadTexture(
    tmpImgPath + "icon_underwater.png", tmpCoorPath + "icon_underwater.txt",
    "icon_underwater");

  loadTexture(
    tmpImgPath + "objects.png", tmpCoorPath + "objects.txt", "objects");

  tmpImgPath  = imgPath + "\\tileset\\";
  tmpCoorPath = coorPath + "\\tileset\\";

  loadTexture(
    tmpImgPath + "tileset_ground.png", tmpCoorPath + "tileset_ground.txt",
    "tileset_ground");

  loadTexture(
    tmpImgPath + "tileset_sky.png", tmpCoorPath + "tileset_sky.txt",
    "tileset_sky");

  loadTexture(
    tmpImgPath + "tileset_underground.png",
    tmpCoorPath + "tileset_underground.txt", "tileset_underground");

  loadTexture(
    tmpImgPath + "tileset_underwater.png",
    tmpCoorPath + "tileset_underwater.txt", "tileset_underwater");

  tmpImgPath  = imgPath + "\\user_interface\\";
  tmpCoorPath = coorPath + "\\user_interface\\";

  loadTexture(tmpImgPath + "buttons.png", tmpCoorPath + "buttons.txt", "UI");

  blocks.merge(tempText);
  tempText.clear();
}

void ResManager::loadMusic() {
  musics["bonus"]      = LoadMusicStream((musicPath + "bonus.ogg").c_str());
  musics["boss"]       = LoadMusicStream((musicPath + "boss.ogg").c_str());
  musics["bowser_battle"]
    = LoadMusicStream((musicPath + "bowser_battle.ogg").c_str());
  musics["castle_theme"]
    = LoadMusicStream((musicPath + "castle_theme.ogg").c_str());
  musics["choose_character"]
    = LoadMusicStream((musicPath + "choose_character.ogg").c_str());
  musics["ending"] = LoadMusicStream((musicPath + "ending.ogg").c_str());
  musics["final_battle"]
    = LoadMusicStream((musicPath + "final_battle.ogg").c_str());
  musics["ground_theme"]
    = LoadMusicStream((musicPath + "ground_theme.ogg").c_str());
  musics["invincibility_theme"]
    = LoadMusicStream((musicPath + "invincibility_theme.ogg").c_str());
  musics["overworld"] = LoadMusicStream((musicPath + "overworld.ogg").c_str());
  musics["title"]     = LoadMusicStream((musicPath + "title.ogg").c_str());
  musics["underground_theme"]
    = LoadMusicStream((musicPath + "underground_theme.ogg").c_str());
  musics["underwater_theme"]
    = LoadMusicStream((musicPath + "underwater_theme.ogg").c_str());
}

void ResManager::loadSounds() {
  sounds["1up"]        = LoadSound((soundPath + "1up.wav").c_str());
  sounds["beep"]         = LoadSound((soundPath + "beep.wav").c_str());
  sounds["billfirework"] = LoadSound((soundPath + "billfirework.wav").c_str());
  sounds["bowserfall"]   = LoadSound((soundPath + "bowserfall.wav").c_str());
  sounds["brick"]        = LoadSound((soundPath + "brick.wav").c_str());
  sounds["bump"]         = LoadSound((soundPath + "bump.wav").c_str());
  sounds["castle_complete"]
    = LoadSound((soundPath + "castle_complete.wav").c_str());
  sounds["coin"]         = LoadSound((soundPath + "coin.wav").c_str());
  sounds["course_clear"] = LoadSound((soundPath + "course_clear.wav").c_str());
  sounds["fire"]         = LoadSound((soundPath + "fire.wav").c_str());
  sounds["fireball"]     = LoadSound((soundPath + "fireball.wav").c_str());
  sounds["flagpole"]     = LoadSound((soundPath + "flagpole.wav").c_str());
  sounds["gameover"]     = LoadSound((soundPath + "gameover.wav").c_str());
  sounds["gameover_unused"]
    = LoadSound((soundPath + "gameover_unused.wav").c_str());
  sounds["hurryup"]   = LoadSound((soundPath + "hurryup.wav").c_str());
  sounds["item"]      = LoadSound((soundPath + "item.wav").c_str());
  sounds["jump"]      = LoadSound((soundPath + "jump.wav").c_str());
  sounds["jumpsmall"] = LoadSound((soundPath + "jumpsmall.wav").c_str());
  sounds["kickkill"]  = LoadSound((soundPath + "kickkill.wav").c_str());
  sounds["level_complete"]
    = LoadSound((soundPath + "level_complete.wav").c_str());
  sounds["life_lost"] = LoadSound((soundPath + "life_lost.wav").c_str());

  sounds["pause"]         = LoadSound((soundPath + "pause.wav").c_str());
  sounds["pipepowerdown"] = LoadSound((soundPath + "pipepowerdown.wav").c_str());
  sounds["powerup"]       = LoadSound((soundPath + "powerup.wav").c_str());
  sounds["stompswim"]     = LoadSound((soundPath + "stompswim.wav").c_str());
  sounds["time-up_warning"]
    = LoadSound((soundPath + "time-up_warning.wav").c_str());
  sounds["vine"]        = LoadSound((soundPath + "vine.wav").c_str());
  sounds["world_clear"] = LoadSound((soundPath + "world_clear.wav").c_str());
}

SaveDatawMap ResManager::loadResourcesFromFile() {
  FileHandler fp;
  std::string path = fp.openFilePath();
  if (path.empty()) {
    return {};
  }
  SaveDatawMap data;
  if (!fp.loadFile(path, data)) {
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
  return characters[key];
}

Texture2D ResManager::getBlock(std::string key) {
  return blocks[key];
}

Music ResManager::getMusic(std::string key) {
  return musics[key];
}

Sound ResManager::getSound(std::string key) {
  return sounds[key];
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