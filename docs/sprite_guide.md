# Sprites guide
- This is a brief guide to show you how to use the sprites. Sprites have two components: 
    + An remapped image file that contains the sprites
    + A text file with the same name that contains the coordinates for each sprite 

- Each line in the text file corresponds to a sprite, from left to right, and from top to bottom, with the following format:

Sprite ID - X Position - Y Position - Width - Height

(X and Y position is anchored on the top left corner of the sprite)

- You can use the ID to keep track of the sprite you want to use, then target its position by letting `std::cin` read the anchor positions and adding to it the width and height to get the size of the rectangle bounding the sprite.

# Update 2/8
I have uploaded a minimal tileset with the bare necessities for drawing a map. I propose changing the AddStaticBlock function as follow. Here is the current StaticBlock function:
```cpp
void ObjectManager::AddStaticBlock(Vector2 position, char theme)
```
The theme parameter here is no longer necessary as we have only a few blocks put in one spritesheet. I suggest changing it to:
```cpp
void ObjectManager::AddStaticBlock(int tileGid, Vector2 position) {
    GameObject* temp = new StaticBlock(tileGid, scale_, position);
      object_list_.push_back(temp);
     collision_->AddObject(temp);
}
```
The StaticBlock (and other block constructors, too) would look something like:
```cpp
StaticBlock::StaticBlock (int tileGid, Vector2 NPosition, float NScale) : GameObject(NPosition, NScale) {
    //call class Map here for consistency 
        texture = map.GetTexture(tileGid);
        frame = map.GetInfo(tileGid);
}
```
I also think we should move the ReadSpriteInfo function in utility.cpp to sprite_manager.cpp to centralize external resource management and simplify the workflow, also because the function is similar to another function named LoadHelper which previously was in sprite_manager and did the same thing. The process now goes:
 sprite_manager (low-level file handling) -> map (sort of a converter to Gid for easier usage) -> object_manager (high-level manager)
