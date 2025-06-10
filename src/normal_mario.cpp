#include "../Mario/NormalMario.h"
NormalMario::NormalMario(Character *Ncontext):CharacterState(10,5,200){
    image=LoadImage("resource/mario.png");
    context=Ncontext;
};