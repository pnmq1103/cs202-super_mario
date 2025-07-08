#include <fstream>
#include <iostream>
#include <stdexcept>

#include "../tinyfiledialogs.h"
#include "include/core/scene.hpp"

void Scene::Resume() {}

void Scene::ReadSpriteInfo(
  const std::string &path, std::unordered_map<int, Rectangle> &sprites) {

  std::ifstream fin;
  fin.open(path);
  if (fin.is_open()) {
    int id;
    float x, y, w, h;
    while (fin >> id) {
      fin >> x >> y >> w >> h;
      sprites[id] = {x, y, w, h};
    }
  } else {
    throw std::runtime_error("invalid file");
  }
}
