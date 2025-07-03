#pragma once
#include <raylib.h>
#include <unordered_map>


enum class BlockType {
    Empty,
    Brick,
    Question,
    Solid,
    Pipe,
    Rock,
    Lava
};

class Block {
protected:
    Vector2 position;
    int width;
    int height;
    BlockType type;
    bool solid;
    int spriteId;

public:
    Block(Vector2 pos, int w, int h, BlockType type, bool solid = true, int spriteId = 0);
    virtual ~Block() = default;

    virtual Block* clone() const = 0;
    
    virtual void update(float dt) {}       
    virtual void onHit();              

    
    Vector2 getPosition() const;
    Rectangle getRect() const;
    BlockType getType() const;
    bool isSolid() const;
    int getSpriteId() const;

    
    void setPosition(Vector2 pos);
    void setSpriteId(int id);
    void setSolid(bool val);

    
    bool checkCollision(Rectangle other) const;

    
    virtual void render(const Texture2D& texture, const std::unordered_map<int, Rectangle>& spriteRects) const;
};
