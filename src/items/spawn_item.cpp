#include "include/items/spawn_item.hpp"
#include "include/items/mushroom_powerup.hpp"
#include "include/items/fire_flower_powerup.hpp"
#include "include/items/star_powerup.hpp"

std::vector<PowerUp*> spawnedItems;

void SpawnItem(PowerUpType type, Vector2 pos) {
  PowerUp* item = nullptr;
  switch (type) {
    case PowerUpType::SuperMushroom:
      item = new MushroomPowerUp(pos); break;
    case PowerUpType::FireFlower:
      item = new FireFlowerPowerUp(pos); break;
    case PowerUpType::Star:
      item = new StarPowerUp(pos); break;
    default: return;
  }
  spawnedItems.push_back(item);
}
