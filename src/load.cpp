#include "load.hpp"

LoadScene::LoadScene() = default;

LoadScene::~LoadScene() = default;

void LoadScene::Init() {
  for (int i = 0; i < 3; ++i) {
    rects[i].x      = 1024/2 - 256 / 2;
    rects[i].y      = 50 + i * (buttonHeight + 50);
    rects[i].width  = buttonWidth;
    rects[i].height = buttonHeight;
  }
}

void LoadScene::Update() {
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
    Vector2 mousePos = GetMousePosition();
    for (int i = 0; i < 3; ++i) {
      if (CheckCollisionPointRec(mousePos, rects[i])) {
        if (saveData[i].mapTiles.size() > 0) { // check if save data exists
          // load game data and switch to game scene
        } 
      }
    }
  }
}

void LoadScene::Draw() {
  DrawText("Choose Saved Game", 1024 / 2 - MeasureText("Choose Saved Game", 40) / 2, 20, 40,
           BLACK);
  for (int i = 0; i < 3; i++) {
    //check for existing saved game
    std::string slotText;
    FileHandler handler(i + 1);
    if (handler.loadGame(saveData[i])) {
      slotText = "Save game no." + std::to_string(i + 1) + " - "
                 + std::to_string(saveData[i].highScore); 
    } else {
      slotText = "Slot " + std::to_string(i+1);
    }
    // draw the rectangle and text for each save slot
    int textWidth = MeasureText(slotText.c_str(), 20);
    DrawRectangleRec(rects[i], LIGHTGRAY);
    DrawRectangleLinesEx(rects[i], 2, DARKGRAY);

    int textX = (int)(rects[i].x + rects[i].width / 2) - textWidth / 2;
    int textY = (int)(rects[i].y + rects[i].height + 10);
    DrawText(slotText.c_str(), textX, textY, 20, BLACK);
  }

}