#include "include/character_state.hpp"
CharacterState::CharacterState(int NrT,float Nspeed,int NjT,float NjH,float Nscale,bool Nleft): rT(NrT),speed(Nspeed),jT(NjT),jH(NjH),scale(Nscale){
    t_x=-1;
    t=0;
    left=Nleft;
    is_jump=false;
    is_fall=false;
    is_die=false;
    disabled=false;
    stop_left=0;
    t_star=0;
}
CharacterState::~CharacterState(){
    UnloadTexture(texture);
}

void CharacterState::LoadFrameList(std::string at){
    std::ifstream fin;
    fin.open(at);
    if (!fin.is_open()) throw std::runtime_error("cannot open file");
    while (!fin.eof()){
        Rectangle rect;
        float n;
        fin>>n;
        fin>>n;
        rect.x=n*scale;
        fin>>n;
        rect.y=n*scale;
        fin>>n;
        rect.width=n*scale;
        fin>>n;
        rect.height=n*scale;
        frame_list.push_back(rect);
    }
    fin.close();
}

Rectangle CharacterState::GetRectangle(){
    Rectangle rect= {pos.x,pos.y,14*scale,27*scale};
    return rect;
}

float CharacterState::GetSpeed(){
    return speed;
}


void CharacterState::SetPosition(Vector2 Npos){
    pos=Npos;
}
void CharacterState::SetFrameCount(){
    ++t;
    if (t_star>0) --t_star;
}


void CharacterState::Draw(){
    std::vector<Color> color_set={RED,ORANGE,YELLOW,GREEN,BLUE,PURPLE,WHITE};
    if (t_star>0){
        DrawTextureRec(texture,frame,pos,color_set[(t_star/5)%7]);
    } else DrawTextureRec(texture,frame,pos,WHITE);
}
void CharacterState::Jump(){
    if (disabled) return;
    if (v==0){
        y=pos.y;
        v=-4.0*jH/jT;
        is_jump=true;
    }
}

void CharacterState::Run(bool left){
    if (disabled) return;
    if (t_x==-1){
        t_x=t;
        this->left=left;
    }
}
void CharacterState::StopX(){
    int n;
    if (left) n=-1; else n=1;

    if (stop_left!=0 && stop_left!=n){
        stop_left=0;
        return;
    }
    if (stop_left==0) stop_left=n;

    t_x=-1; 
    stop_x=pos.x;
}
void CharacterState::StopY(float Ny){
    if (is_die) return;
    if (is_jump) {
        is_jump=false;
        return;
    }
    is_fall=false;
    v=-8.0*jH/(jT*jT);
    pos.y=Ny;  

    frame=frame_list[0];
    if (left) {
        frame.width=-frame.width;
    } 
}
void CharacterState::StopY(){
    v=0;
    is_fall=true;
}
bool CharacterState::IsFalling(){
    return is_fall;
}


void CharacterState::Update(){

    if (stop_left==1) {
        if (pos.x>stop_x) stop_left=0;
    }  else if (stop_left==-1){
        if (pos.x<stop_x) stop_left=0;
    } 

    float a=8.0*jH/(jT*jT);
    v+=a;
    pos.y+=v;

    if (v>0) is_fall=true;
    

     
    if (t_x!=-1){
        if (v==0){
            if (t-t_x==rT) {
                t_x=-1; 
            } 
            if (left) pos.x-=speed; else pos.x+=speed;
        } else {
            t_x=-1;
            if (left) pos.x-=speed; else pos.x+=speed;
        }
    } 

}
void CharacterState::ToStarman(){
    if (disabled) return;
    t_star=60*10;
}
bool CharacterState::IsStarman(){
    if (t_star==0) return false; else return true;
}