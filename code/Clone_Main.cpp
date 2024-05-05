#include "Clone_Main.h"

struct Entity 
{
    int id;

    Texture2D Sprite;
    Rectangle SrcRect;
    Rectangle DstRect;

    real32 Scale;
    real32 Rotation;
    vec2 pos;
    vec2 vel;

    real32 MovementSpeed;
    real32 Accel;
    real32 Friction;
};

internal vec2 
HandlePlayerMovement(Entity *Player) 
{
    if(IsKeyDown(KEY_W)) 
    {
        Player->vel.y = -Player->MovementSpeed;
    }
    else if(IsKeyDown(KEY_S)) 
    {
        Player->vel.y = Player->MovementSpeed;
    }
    else 
    {
        Player->vel.y = 0;
    }

    if(IsKeyDown(KEY_A)) 
    {
        Player->vel.x = -Player->MovementSpeed;
    }
    else if(IsKeyDown(KEY_D)) 
    {
        Player->vel.x = Player->MovementSpeed;
    }
    else 
    {
        Player->vel.x = 0;
    }

    Player->pos.x += Player->vel.x * GetFrameTime();
    Player->pos.y += Player->vel.y * GetFrameTime();

    return(Player->pos);
}

int main() 
{
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Hello, Window");

    Entity Player;

    Player.Sprite = CreateTextureFromFilepath("../data/res/textures/PlayerCharacter.png");
    Player.pos = {200, 300};
    Player.Rotation = 0.0f;
    Player.Scale = 4.0f;
    Player.SrcRect = {0, 0, 16, 32};
    Player.MovementSpeed = 350;
    Player.Accel = 5.0f;
    Player.Friction = 2.5f;
    Player.id = 0;

    while(!WindowShouldClose()) 
    {
        BeginDrawing();
        ClearBackground(BLUE);
    
        vec2 NewPos = HandlePlayerMovement(&Player);
        Player.pos = NewPos;
        Player.DstRect = {Player.pos.x, Player.pos.y, Player.SrcRect.width * Player.Scale, Player.SrcRect.height * Player.Scale};
        DrawTexturePro(Player.Sprite, Player.SrcRect, Player.DstRect, {0, 0}, Player.Rotation, WHITE);

        EndDrawing();
    }
    CloseWindow();

    return(0);
}
