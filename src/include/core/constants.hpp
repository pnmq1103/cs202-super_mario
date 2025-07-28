#pragma once

namespace constants {
  // The scale factor for the whole program is 4x
  inline constexpr float scale        = 4.0f;
  inline constexpr float screenWidth  = 256.0f * scale;
  inline constexpr float screenHeight = 240.0f * scale;
  inline constexpr float blockSize    = 16.0f * scale; // int needs typecast
  inline constexpr float cellSize     = 16;

  inline constexpr int mapWidth  = 250; // in blocks
  inline constexpr int mapHeight = 28;
} // namespace constants