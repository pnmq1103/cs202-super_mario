#include <raylib.h>
#include "src/include/character.hpp"
#include <vector>
#include <iostream>
int main(){
    InitWindow(3000,1000,"super mario");
    SetTargetFPS(60);
    Character character(5);
    character.setCharacter(MARIO,{1000,500});

    Rectangle rect=character.getRectangle();
    Rectangle rect1={2000,500+rect.height-100,100,100};
    float mark=1000;
    bool check=false;

    while (!WindowShouldClose()){
        BeginDrawing();
        ClearBackground(BLACK);
        DrawRectangleRec(rect1,BLUE);


        character.setFrameCount();
        

        rect=character.getRectangle();
        float speed=character.getSpeed();
        
        
        if (rect.y>=500) character.stopY(500);
        if ((rect.x+abs(rect.width)<2000) || (rect.x>=2100)){
           
        } else {
            if (rect.y>400 && !character.isFalling()) {
                mark=rect.x;
                character.stopX();
            }
            if (rect.y>=400 && rect.x!=mark) {
                character.stopY(400);
            }

        }

        character.update();
        character.draw();

        if (IsKeyDown(KEY_LEFT)) character.run(true);
        else if (IsKeyDown(KEY_RIGHT)) character.run(false);
        if (IsKeyDown(KEY_UP)) {
            character.jump();
        }



        EndDrawing();
    }
    
    CloseWindow();
    return 0;
}