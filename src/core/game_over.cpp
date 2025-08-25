#include "include/core/game_over.hpp"

void GameOverScene::Init() {
  // Initialization code for Game Over scene
  game_over_image = LoadImageAnim("res/game_over.gif", &anim_frames);
  game_over_texture = LoadTextureFromImage(game_over_image);
}

void GameOverScene::Update() {
  if (IsKeyPressed(KEY_ESCAPE)) {
	App.RemoveScene();
	return;
  }
  frame_counter++;
  if (frame_counter >= 7) {
    current_frame++;
    if (current_frame >= anim_frames) current_frame = 0; // Loop back to the first frame
    next_frame_offset = game_over_image.width * game_over_image.height * 4 * current_frame;
    UpdateTexture(
      game_over_texture, (unsigned char *)game_over_image.data + next_frame_offset);
    frame_counter = 0;
  }

}

void GameOverScene::Draw() {
  ClearBackground(BLACK);
  DrawTexture(game_over_texture, constants::screenWidth / 2 - 133, constants::screenHeight / 2 - 150, WHITE);
  DrawText(
    "GAME OVER... Press ESC to return", constants::screenWidth / 2 - 250,
    constants::screenHeight / 2 + 200, 30, RED);
}

void GameOverScene::Resume() {}

SceneType GameOverScene::Type() {
  return type_;
}