#pragma once
#include <fstream>
#include <raylib.h>
#include <stdexcept>
#include <string>
#include <unordered_map>

namespace utility {
  void ReadSpriteInfo(
    const std::string &path, std::unordered_map<int, Rectangle> &sprites);

} // namespace utility