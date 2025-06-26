#pragma once
#include "include/character.hpp"
class CharacterState{
    protected:
        float y,v,stopX;
        int tX,t,stopLeft;
        bool left,isJump,isFall;
        
        Character *context;
        Vector2 pos;
        Texture2D texture;
        const float speed,jH,scale;
        const int jT;
        Rectangle frame;
    public:
        CharacterState(float Nspeed,int NjT,float NjH,float Nscale);
        virtual ~CharacterState();

        Rectangle getRectangle();
        void setPosition(Vector2 Npos);
        void setFrameCount();
        float getSpeed();
        
        virtual void draw();
        void run(bool left);
        void jump();
        virtual void update()=0;
        void StopX();
        void StopY(float Ny);
        void StopY();
        bool isFalling();
};