#include "raylib.h"
#include "raymath.h"
#include "Character.h"
#include "Prop.h"
#include "Enemy.h"
#include <string>

int main()
{
    // window size
    const int windowDimensions[2]{384, 384};
    // initialize window
    InitWindow(windowDimensions[0], windowDimensions[1], "Luke's Top-Down");
    // Load map textures
    Texture2D map = LoadTexture("nature_tileset/OpenWorldMap24x24.png");
    Vector2 mapPos{0.0, 0.0};
    const float mapScale{4.0};

    Character knight{windowDimensions[0], windowDimensions[1]};


    Prop props[2]{
        Prop{Vector2{600.f, 300.f}, LoadTexture("nature_tileset/Rock.png")},
        Prop{Vector2{400.f, 500.f}, LoadTexture("nature_tileset/Log.png")}
    };
    
    
    Enemy goblin{Vector2{800.f,800.f}, LoadTexture("characters/goblin_idle_spritesheet.png"), LoadTexture("characters/goblin_run_spritesheet.png")};
    Enemy slime{Vector2{1200.f,1200.f}, LoadTexture("characters/slime_idle_spritesheet.png"), LoadTexture("characters/slime_run_spritesheet.png")};
    
    Enemy* enemies[]{
        &goblin,
        &slime
    };
    for( auto enemy : enemies)
    {
        enemy->setTarget(&knight);
    }

    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(WHITE);

        mapPos = Vector2Scale(knight.getWorldPos(), -1.f);

        // draw the map
        DrawTextureEx(map, mapPos, 0.0, mapScale, WHITE);


        // draw props
        for(auto prop : props)
        {
            prop.Render(knight.getWorldPos());
        }

        if (!knight.getAlive()) // character is not alive
        {
            DrawText("Game Over!", 55.f, 45.f, 40, RED);
            EndDrawing();
            continue;
        }
        else // character is alive
        {
            std::string knightsHealth = "health: ";
            knightsHealth.append(std::to_string(knight.getHealth()),0 ,5 );
            DrawText(knightsHealth.c_str(), 55.f, 45.f, 40, RED);
        }

        knight.tick(GetFrameTime());
        // check map bounds
        if (knight.getWorldPos().x < 0.f ||
        knight.getWorldPos().y < 0.f ||
        knight.getWorldPos().x + windowDimensions[0]> map.width * mapScale ||
        knight.getWorldPos().y + windowDimensions[1]> map.height * mapScale)
        {
            knight.undoMovement();
        }

        // check prop collisions
        for(auto prop : props)
        {
            if(CheckCollisionRecs (prop.GetCollisionRec(knight.getWorldPos()) , knight.GetCollisionRec()))
            {
                knight.undoMovement();
            }
        }

        for( auto enemy : enemies){
        enemy->tick(GetFrameTime());

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            if (CheckCollisionRecs(enemy->GetCollisionRec(), knight.getWeaponCollisionRec()))
            {
                enemy->setAlive(false);
            }
        }
        }

        EndDrawing();
    }
    UnloadTexture(map);
    CloseWindow();
}