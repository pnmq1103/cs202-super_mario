#include "include/character_state.hpp"
#include <iostream>
CharacterState::CharacterState(float Nspeed,int NjT,float NjH,float Nscale): speed(Nspeed),jT(NjT),jH(NjH),scale(Nscale){
    tX=-1;
    t=0;
    left=false;
    isJump=false;
    isFall=false;
    stopLeft=0;
}
CharacterState::~CharacterState(){
    UnloadTexture(texture);
}

Rectangle CharacterState::getRectangle(){
    Rectangle rect= {pos.x,pos.y,14*scale,27*scale};
    return rect;
}

float CharacterState::getSpeed(){
    return speed;
}


void CharacterState::setPosition(Vector2 Npos){
    pos=Npos;
}
void CharacterState::setFrameCount(){
    ++t;
}


void CharacterState::draw(){
    DrawTextureRec(texture,frame,pos,WHITE);
}
void CharacterState::jump(){
    if (v==0){
        y=pos.y;
        v=-4.0*jH/jT;
        isJump=true;
    }
}

void CharacterState::run(bool left){
    if (tX==-1){
        tX=t;
        this->left=left;
    }
}
void CharacterState::StopX(){
    int n;
    if (left) n=-1; else n=1;

    if (stopLeft!=0 && stopLeft!=n){
        stopLeft=0;
        return;
    }
    if (stopLeft==0) stopLeft=n;

    tX=-1; 
    stopX=pos.x;
}
void CharacterState::StopY(float Ny){
    if (isJump) {
        isJump=false;
        return;
    }
    isFall=false;
    v=-8.0*jH/(jT*jT);
    pos.y=Ny;  

    frame={254*scale,0,14*scale,27*scale};
    if (left) {
        frame.width=-frame.width;
    } 
}
void CharacterState::StopY(){
    v=0;
    isFall=true;
}
bool CharacterState::isFalling(){
    return isFall;
}