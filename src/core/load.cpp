#include <raylib.h>
#include <string>

#include "include/core/application.hpp"
#include "include/core/load.hpp"

LoadScene::LoadScene() {
  for (int i = 0; i < 3; ++i) {
    rects[i].x      = 1024 / 2 - 256 / 2;
    rects[i].y      = static_cast<float>(150 + i * (buttonHeight + 50));
    rects[i].width  = static_cast<float>(buttonWidth);
    rects[i].height = static_cast<float>(buttonHeight);
  }
}

LoadScene::~LoadScene() = default;

void LoadScene::Init() {}

void LoadScene::Update() {
  if (IsKeyPressed(KEY_ESCAPE)) {
    App.RemoveScene();
    return;
  }
  // int selectedSlot    = 0;
  // bool waitingForPath = false;
  // path;
  // if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
  //   Vector2 m = GetMousePosition();
  //   for (int i = 0; i < 3; ++i) {
  //     if (CheckCollisionPointRec(m, rects[i])) {
  //       // fix bug by moving file opening from draw to update
  //       selectedSlot   = i;
  //       waitingForPath = true;
  //     }
  //   }
  // }
  // if (waitingForPath) {
  //   FileHandler handler(selectedSlot + 1);
  //   path = handler.OpenFilePath();
  //   // handler.LoadFile(path, saveData[selectedSlot]);
  //   waitingForPath = false;
  // }
}

void LoadScene::Draw() {
  DrawText(
    "Choose Saved Game", 1024 / 2 - MeasureText("Choose Saved Game", 40) / 2,
    20, 40, BLACK);
  // for (int i = 0; i < 3; i++) {
  //   // check for existing saved game
  //   std::string slotText;
  //   FileHandler handler(i + 1);
  //   if (handler.LoadFile(path, saveData[i])) {
  //     slotText = "Save game no." + std::to_string(i + 1) + " - "
  //                + std::to_string(saveData[i].score);
  //   } else {
  //     slotText = "Slot " + std::to_string(i + 1);
  //   }
  //   // draw the rectangle and text for each save slot
  //   int textWidth = MeasureText(slotText.c_str(), 20);
  //   DrawRectangleRec(rects[i], LIGHTGRAY);
  //   DrawRectangleLinesEx(rects[i], 2, DARKGRAY);

  //  int textX = (int)(rects[i].x + rects[i].width / 2) - textWidth / 2;
  //  int textY = (int)(rects[i].y + rects[i].height + 10);
  //  DrawText(slotText.c_str(), textX, textY, 20, BLACK);
  //}
}

void LoadScene::Resume() {}

SceneType LoadScene::Type() {
  return type_;
}
