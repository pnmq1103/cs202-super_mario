#include "../Character/Character.h"
#include "../Mario/NormalMario.h"
#include <iostream>
Character::Character(){
    pState=nullptr;
    state=0;
}
Character::~Character(){
    if (pState) delete pState;
}
void Character::setCharacter(CharacterType Ntype,Vector2 pos,Vector2 size){
    type=Ntype;
    setState(0);
    pState->setPosition(pos);
    pState->setSize(size);
}
void Character::setState(int state){
    if (type==MARIO){
        switch(state){
            case 0:
                pState=new NormalMario(this);
                break;
        }
    } else if (type==LUIGI){

    }
}
void Character::draw(){
    pState->draw();
}
void Character::run(bool left){
    pState->run(left);
}
void Character::jump(){
    pState->jump();
}
void Character::updateJump(){
    pState->updateJump();
}


CharacterState::CharacterState(double Nspeed,double NjT,double NjH): speed(Nspeed),jT(NjT),jH(NjH){
    t=-1;
}
CharacterState::~CharacterState(){
    UnloadImage(image);
}

Vector2 CharacterState::getPosition(){
    return pos;
}
Vector2 CharacterState::getSize(){
    return size;
} 
void CharacterState::setPosition(Vector2 Npos){
    pos=Npos;
}
void CharacterState::setSize(Vector2 Nsize){
    size=Nsize;
}


void CharacterState::draw(){
    ImageResize(&image,size.x,size.y);
    Texture2D tex=LoadTextureFromImage(image);
    DrawTexture(tex,pos.x,pos.y,WHITE);
}
void CharacterState::run(bool left){
    if (left) pos.x-=speed; else pos.x+=speed;
}
void CharacterState::jump(){
    if (t==-1){
        t=0;
        y=pos.y;
    }
}
void CharacterState::updateJump(){
    if (t>jT) t=-1;
    else if (t>=0){
        t+=0.1;
        double a=8.0*jH/(jT*jT), v=-4.0*jH/jT;
        std::cout<<"a "<<a<<" "<<v<<'\n';
        pos.y=y+v*t+a*t*t/2.0;
        std::cout<<t<<"  "<<pos.y<<'\n';
    } else if (t>jT) t=-1;
}