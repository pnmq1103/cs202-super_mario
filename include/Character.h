#pragma once
#include <raylib.h>
enum CharacterType{
    MARIO,
    LUIGI
};
class CharacterState;
class Character{
    private:
        CharacterState *pState;
        CharacterType type;
        int state;
    public:
        Character();
        ~Character();
        void setCharacter(CharacterType type,Vector2 pos,Vector2 size);
        void setState(int state);
        void draw();
        void run(bool left);
        void jump();
        void updateJump();
};
class CharacterState{
    private:
        double y;
        double t;
    protected:
        Character *context;
        Vector2 pos,size;
        Image image;
        const double speed, jT,jH;
    public:
        CharacterState(double Nspeed,double NjT,double NjH);
        ~CharacterState();

        Vector2 getPosition();
        Vector2 getSize(); 
        void setPosition(Vector2 Npos);
        void setSize(Vector2 Nsize);
        
        virtual void draw();
        void run(bool left);
        void jump();
        void updateJump();
};