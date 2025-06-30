#include "include/normal_luigi.hpp"
#include <iostream>
#include <math.h>
NormalLuigi::NormalLuigi(Character *Ncontext,float Nscale,bool Nleft,bool tran):CharacterState(18,5,50,500,Nscale,Nleft){
    if (!tran){
        disabled=true;
        Image image=LoadImage("res/sprites/characters/luigi_starup.png");
        ImageResize(&image,image.width*scale,image.height*scale);
        pre_texture_=LoadTextureFromImage(image);
        UnloadImage(image);
        pre_frame_={254*scale, 0, 14*scale, 27*scale};
    }
    Image image=LoadImage("res/sprites/characters/luigi_normal.png");
    ImageResize(&image,image.width*scale,image.height*scale);
    texture=LoadTextureFromImage(image);
    UnloadImage(image);
    context=Ncontext;
    LoadFrameList("res/sprites/characters/normal.txt");
    frame=frame_list[0];
};
NormalLuigi::~NormalLuigi(){}
void NormalLuigi::Die(){
    if (disabled) return;
    CharacterState::Jump();
    is_die=true;
}
void NormalLuigi::Evolve(){
    if (disabled) return;
    context->SetState(1,true);
}
void NormalLuigi::Update(){
    if (disabled) return;
    CharacterState::Update();
    if (v!=0){
        frame=frame_list[8];        
    }

    if (t_x!=-1 && t-t_x!=rT && v==0){
        frame=frame_list[(t-t_x)/(rT/2)+3]; 
    } 

    if (left) {
        frame.width=-abs(frame.width);
    } else frame.width=abs(frame.width);
}
void NormalLuigi::Draw(){
    if (disabled){
        if (t/5%2==0){
            if (left) {
                frame.width=-abs(frame.width);
            } else frame.width=abs(frame.width);
            DrawTextureRec(texture,frame,pos,WHITE);
        } else {
            if (left) {
                pre_frame_.width=-abs(pre_frame_.width);
            } else pre_frame_.width=abs(pre_frame_.width);
            DrawTextureRec(pre_texture_,pre_frame_,pos,WHITE);
        }
        if (t==30) {
            disabled=false;
            UnloadTexture(pre_texture_);
        }
    } else {
        CharacterState::Draw();
    }
}
