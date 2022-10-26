#include "raylib.h"

struct AnimData 
{
Rectangle rec; 
Vector2 pos;
int frame; 
float updateTime;
float runningTime;
};

bool isOnGround(AnimData data, int windowHeight)
{
    return data.pos.y >= windowHeight - data.rec.height;
}

AnimData updateAnimData(AnimData data, float deltaTime, int maxFrame)
{
    // update running time
    data.runningTime += deltaTime;
    if (data.runningTime >= data.updateTime)
    {
        data.runningTime = 0.0;
        // update animation frame
        data.rec.x = data.frame * data.rec.width;
        data.frame ++;
        if (data.frame > maxFrame)
        {
            data.frame = 0;
        }
    }
    return data;
}

int main()
{
    //window dimensions
    int windowDimensions[2];
    windowDimensions[0] = 600;
    windowDimensions[1] = 500;
    
    // initializing window
    InitWindow(windowDimensions[0], windowDimensions[1], "Dapper");

    // acceleration due to gravity (pixels/second)/second
    const int gravity{2'000};

    // nebula variables 
    Texture2D nebula = LoadTexture( "textures/12_nebula_spritesheet.png");

    // AnimData for nebula
    const int sizeOfNebulae{6};
    AnimData nebulae[sizeOfNebulae]{};

    for (int i = 0; i < sizeOfNebulae; i++)
    {
        nebulae[i].rec.x = 0.0;
        nebulae[i].rec.y = 0.0;
        nebulae[i].rec.width = nebula.width/8;
        nebulae[i].rec.height = nebula.height/8;
        nebulae[i].pos.y = windowDimensions[1] - nebula.height/8;
        nebulae[i].frame = 0;
        nebulae[i].runningTime = 0.0;
        nebulae[i].updateTime = 1.0/12.0;
        nebulae[i].pos.x = windowDimensions[0] + i*400;
    }

    float finishLine{ nebulae[sizeOfNebulae - 1].pos.x };
    
    // nebula X velocity (pixels/second)
    int nebVel{-500};
    
    // loading and initialising scarfy
    Texture2D scarfy = LoadTexture("textures/scarfy.png");
    AnimData scarfyData;
    scarfyData.rec.width = scarfy.width/6;
    scarfyData.rec.height = scarfy.height;
    scarfyData.rec.x = 0;
    scarfyData.rec.y = 0;
    scarfyData.pos.x = windowDimensions[0]/2 - scarfyData.rec.width/2;
    scarfyData.pos.y = windowDimensions[1] - scarfyData.rec.height;
    scarfyData.frame = 0;
    scarfyData.updateTime = 1.0/12.0;
    scarfyData.runningTime = 0.0;


    // is rectangle in the air
    bool isInAir{};
    // jump velocity pixels/second
    const int jumpVel{-800};
    

    int velocity{ 0 }; //pixels/frame

    Texture2D background = LoadTexture("textures/far-buildings.png");
    float bgX{};
    Texture2D midground = LoadTexture("textures/back-buildings.png");
    float mgX{};
    Texture2D foreground = LoadTexture("textures/foreground.png");
    float fgX{};

    bool collision{};

    SetTargetFPS(60);
    while(!WindowShouldClose()){
        // start drawing
        BeginDrawing();
        ClearBackground(WHITE);
        const float dT{ GetFrameTime()};

        bgX -= 20 * dT;
        if (bgX <= -background.width*2)
        {
            bgX = 0.0;
        }
        // scroll the midground
        mgX -= 40 * dT;
        if (mgX <= -midground.width*2)
        {
            mgX = 0.0;
        }
        // scroll the foreground
        fgX -= 80 * dT;
        if (fgX <= -foreground.width*2)
        {
            fgX = 0.0;
        }

        Vector2 bgPos{bgX, 0.0};
        DrawTextureEx(background, bgPos, 0.0, 3.0, WHITE);
        Vector2 bg2Pos{bgX + background.width*2, 0.0};
        DrawTextureEx(background, bg2Pos, 0.0, 3.0, WHITE);

        Vector2 mgPos{mgX, 0.0};
        DrawTextureEx(midground, mgPos, 0.0, 3.0, WHITE);
        Vector2 mg2Pos{mgX + midground.width*2, 0.0};
        DrawTextureEx(midground, mg2Pos, 0.0, 3.0, WHITE);

        Vector2 fgPos{fgX, 0.0};
        DrawTextureEx(foreground, fgPos, 0.0, 3.0, WHITE);
        Vector2 fg2Pos{fgX + foreground.width*2, 0.0};
        DrawTextureEx(foreground, fg2Pos, 0.0, 3.0, WHITE);
 



        // ground check
        if (isOnGround(scarfyData, windowDimensions[1]))
        {
            velocity = 0;
            isInAir = false;
        }
        else
        {
            // rectangle is in the air
            velocity += gravity * dT;
            isInAir = true;
        }

        // check for jumping
        if(IsKeyPressed(KEY_SPACE) && !isInAir)
        {
            velocity += jumpVel;
        }

        //update nebula position
        for (int i = 0; i < sizeOfNebulae; i++)
        {
            nebulae[i].pos.x += nebVel * dT;
        }

        // update finish line
        finishLine += nebVel * dT;

        // update scarfy position
        scarfyData.pos.y += velocity * dT;

        if ( !isInAir)
        {
            // update scarfy animation frame
            scarfyData = updateAnimData(scarfyData, dT, 5);
        }

        for (int i = 0; i < sizeOfNebulae; i++)
        {
            // update nebula animation frame
           nebulae[i] = updateAnimData(nebulae[i], dT, 8);
        }

        

        
        for (AnimData nebula : nebulae)
        {
            float pad{50};
            Rectangle nebRec {
                nebula.pos.x + pad,
                nebula.pos.y + pad,
                nebula.rec.width - 2*pad,
                nebula.rec.height - 2*pad
            };
            Rectangle scarfyRec{
                scarfyData.pos.x,
                scarfyData.pos.y,
                scarfyData.rec.width,
                scarfyData.rec.height
            };
            if (CheckCollisionRecs(nebRec, scarfyRec))
            {
                collision = true;
            }
        }
        if(collision)
        {
            //lose the game
            DrawText("Game Over!", windowDimensions[0]/4 - 50, windowDimensions[1]/2, 50, RED);
        }
        else if(scarfyData.pos.x >= finishLine)
        {
            // win the game
            DrawText("You Win!", windowDimensions[0]/4 - 50, windowDimensions[1]/2, 50, GREEN);
        }
        
        else
        {
        // draw nebulae
            for (int i = 0; i < sizeOfNebulae; i++){
                DrawTextureRec(nebula, nebulae[i].rec, nebulae[i].pos, WHITE);
            }
            // Draw scarfy
            DrawTextureRec(scarfy, scarfyData.rec, scarfyData.pos, WHITE);

        }

        // stop drawing
        EndDrawing();
    }
    UnloadTexture(scarfy);
    UnloadTexture(nebula);
    UnloadTexture(background);
    UnloadTexture(midground);
    UnloadTexture(foreground);
    CloseWindow();


}