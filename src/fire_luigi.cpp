#include "include/fire_luigi.hpp"
FireLuigi::FireLuigi(Character *Ncontext,float Nscale,bool Nleft,bool tran):CharacterState(18,5,50,500,Nscale,Nleft){
    LoadFrameList("res/sprites/characters/starup.txt");
    if (tran){
        disabled=true;
        Image image=LoadImage("res/sprites/characters/luigi_starup.png");
        ImageResize(&image,image.width*scale,image.height*scale);
        pre_texture_=LoadTextureFromImage(image);
        UnloadImage(image);
        pre_frame_=frame_list[0];
    }
    Image image=LoadImage("res/sprites/characters/luigi_starup_fire.png");
    ImageResize(&image,image.width*scale,image.height*scale);
    texture=LoadTextureFromImage(image);
    UnloadImage(image);
    context=Ncontext;
    frame=frame_list[0];
};
FireLuigi::~FireLuigi(){}
void FireLuigi::Die(){
    if (disabled) return;
    context->SetState(1,false);
}
void FireLuigi::Evolve(){}
void FireLuigi::Update(){
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
void FireLuigi::Draw(){
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