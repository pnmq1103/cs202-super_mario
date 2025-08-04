#pragma once

#include "include/collision/collision_handler.hpp"
#include "include/objects/brick_block.hpp"
#include "include/objects/coin.hpp"
#include "include/objects/fire_flower.hpp"
#include "include/objects/pipe_block.hpp"
#include "include/objects/question_block.hpp"
#include "include/objects/static_block.hpp"
#include "include/objects/super_mushroom.hpp"
#include "include/objects/super_star.hpp"

class ObjectManager {
private:
  std::vector<GameObject *> object_list_;
  float scale_;
  CollisionHandler *collision_;

  ObjectManager();
  ObjectManager(const ObjectManager &)            = delete;
  ObjectManager &operator=(const ObjectManager &) = delete;

public:
  static ObjectManager &GetInstance();
  ~ObjectManager();

  void Reset(int Nscale, CollisionHandler *Ncollision);
  void DeleteObject(int index);
  void AddBrickBlock(Vector2 position);
  void AddCoin(Vector2 position);
  void AddFireFlower(Vector2 position);
  void AddPipeBlock(
    Vector2 position, int length, bool is_vertical, bool has_head,
    bool has_tail);
  void AddQuestionBlock(Vector2 position, QuestionBlockItem item);
  void AddStaticBlockByGid(Vector2 position, Rectangle rect);
  void AddStaticBlockByTheme(Vector2 position, char theme);
  void AddSuperMushroom(Vector2 position, bool to_left);
  void AddSuperStar(Vector2 position);

  void SetFrameCount();
  void Update();
  void Draw();
  void OnHit(int index); // this is for testing purpose only. please dont use
                         // this function.
};