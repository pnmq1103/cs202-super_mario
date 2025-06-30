#include "include/big_luigi.hpp"
BigLuigi::BigLuigi(Character *Ncontext,float Nscale,bool Nleft,bool tran):CharacterState(18,5,50,500,Nscale,Nleft){
    disabled=true;
    Image image=LoadImage("res/sprites/characters/luigi_starup.png");
    ImageResize(&image,image.width*scale,image.height*scale);
    texture=LoadTextureFromImage(image);
    UnloadImage(image);
    context=Ncontext;
    LoadFrameList("res/sprites/characters/starup.txt");
    frame=frame_list[0];

    if (tran){
        image=LoadImage("res/sprites/characters/luigi_normal.png");
        pre_frame_={0,0,12*scale,15*scale};
    } else {
        image=LoadImage("res/sprites/characters/luigi_starup_fire.png");
        pre_frame_=frame_list[0];
    }
    ImageResize(&image,image.width*scale,image.height*scale);
    pre_texture_=LoadTextureFromImage(image);
    UnloadImage(image);
    
}
BigLuigi::~BigLuigi(){}
void BigLuigi::Die(){
    if (disabled) return;
    context->SetState(0,false);
}
void BigLuigi::Evolve(){
    if (disabled) return;
    context->SetState(2,true);
}
void BigLuigi::Update(){
    if (disabled) return;
    CharacterState::Update();
    float v0=4.0*jH/jT;
    if (v!=0){
        if (v<-v0*1/3){
            frame=frame_list[6];
        }  else if (v>v0*1/3){
            frame=frame_list[8];
        }  else {
            frame=frame_list[7];
        } 
    }

    if (t_x!=-1 && t-t_x!=rT && v==0){
        frame=frame_list[(t-t_x)/(rT/3)+3]; 
    } 

    if (left) {
        frame.width=-abs(frame.width);
    } else frame.width=abs(frame.width);
}
void BigLuigi::Draw(){
    if (t<=30){
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
        if (t==30) UnloadTexture(pre_texture_);
    } else {
        disabled=false;
        CharacterState::Draw();
    }
}