#pragma once
#include "Character.hpp"
class CharacterState{
    protected:
        float y;
        int tX,tY,t;
        bool left;
        Character *context;
        Vector2 pos;
        Texture2D texture;
        const float speed,jH,scale;
        const int jT;
        Rectangle frame;
    public:
        CharacterState(float Nspeed,int NjT,float NjH,float Nscale);
        ~CharacterState();

        Vector2 getPosition();
        Vector2 getSize(); 
        void setPosition(Vector2 Npos);
        void setFrameCount();
        
        virtual void draw();
        void run(bool left);
        void jump();
        virtual void update()=0;
};