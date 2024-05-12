#include "Clone_Main.h"

//#define DEBUG

// NOTE : Perhaps we could decide which entity type something is through a simple enum?
// For example, Set the enum to PLAYER to create a PLAYER Entity?
// Perhaps set it to GOBLIN to create a GOBLIN entity?
//
// And yes I know Block Comments exist. Fuck you.

enum AnimationState 
{
    IDLE,
    WALKING,
    JUMPING,
};

enum EntityType 
{
    PLAYER,
    ENEMY,
    MAPOBJECT,
};

// TODO : Check if adding these into an array rather than initializing them as so
// would be more effective when indexing for an animation:
//
// struct Animation {};
//
// struct HurtAnimation : public Aniamtion 
// {
//      ...DATA
// };
//
// Animation *Animations;
// Animations[0] = HurtAnimation

struct Animation 
{
    int StartingFrame;
    int EndingFrame;
    int CurrentFrame;
    int FrameTime;
    int FrameDelay;
};    

struct IdleAnimation : public Animation {};
struct WalkingAnimation : public Animation {};

struct EntityFlags 
{
    bool IsMoving;
    bool Flipped;
    bool IsOnGround;
};

struct Entity 
{
    EntityFlags Flags;

    Texture2D Sprite;
    Rectangle SrcRect;
    Rectangle DstRect;

    EntityType EntityType;
    AnimationState AnimationState;

    IdleAnimation IdleAnimation;
    WalkingAnimation WalkingAnimation;

    int SpriteFrames;
    int CurrentFrame;
    int WalkingDelay;
    int32 FrameTime;
    
    real32 MovementSpeed;
    real32 Rotation;
    real32 Scale;
    vec2 Pos, Vel;
};

struct State 
{
    Camera2D Camera;

    int TextureCount;
    Texture2D *Textures;
    
    struct Entity 
    {
        EntityFlags Flags;

        Texture2D Sprite;
        Rectangle SrcRect;
        Rectangle DstRect;
        int SpriteFrames;

        EntityType EntityType;
        AnimationState AnimationState;
        IdleAnimation IdleAnimation;
        WalkingAnimation WalkingAnimation;
        
        real32 MovementSpeed;
        real32 Rotation;
        real32 Scale;
        vec2 Pos, Vel;
    }Player;

    struct Input 
    {
        struct Up 
        {
            bool Pressed;
            bool Down;
        }Up;

        struct Down 
        {
            bool Pressed;
            bool Down;
        }Down;

        struct Left
        {
            bool Pressed;
            bool Down;
        }Left;

        struct Right
        {
            bool Pressed;
            bool Down;
        }Right;
    }Input;
};

internal State 
InitGameData(State *State) 
{
    State->Textures = {};
    State->TextureCount = 1;
    State->Textures = (Texture2D *)malloc(int(sizeof(Texture2D)) * State->TextureCount);
    State->Textures[0] = LoadTexture("../data/res/textures/PlayerCharacter.png");
    State->Player.Sprite = State->Textures[0]; 

    State->Player.EntityType = PLAYER;
    State->Player.AnimationState = IDLE;
    State->Player.SpriteFrames = 9;
    State->Player.Flags.Flipped = false;

    State->Player.IdleAnimation.StartingFrame = 0;
    State->Player.IdleAnimation.EndingFrame = 1;
    State->Player.IdleAnimation.CurrentFrame = 0;
    State->Player.IdleAnimation.FrameDelay = 100;
    State->Player.IdleAnimation.FrameTime = 0;

    State->Player.WalkingAnimation.StartingFrame = 4;
    State->Player.WalkingAnimation.EndingFrame = 8;
    State->Player.WalkingAnimation.CurrentFrame = 4;
    State->Player.WalkingAnimation.FrameDelay = 20;
    State->Player.WalkingAnimation.FrameTime = 0;

    State->Player.Pos = {200, 300};
    State->Player.Vel = {0, 0};
    State->Player.Rotation = 0;
    State->Player.Scale = 0;
    State->Player.MovementSpeed = 50.0f;

    return(*State);
}

internal void 
HandlePlayerInput(State *State) 
{
// TODO : Replace this with the State->Input struct's functionality

    if(IsKeyDown(KEY_W)) 
    {
        State->Player.Vel.y = -State->Player.MovementSpeed;
        State->Player.AnimationState = WALKING;
    }
    else if(IsKeyDown(KEY_S)) 
    {
        State->Player.Vel.y = State->Player.MovementSpeed;
        State->Player.AnimationState = WALKING;
    }

    if(IsKeyDown(KEY_A)) 
    {
        State->Player.Vel.x = -State->Player.MovementSpeed;
        State->Player.AnimationState = WALKING;
        State->Player.Flags.Flipped = true;
    }
    else if(IsKeyDown(KEY_D)) 
    {
        State->Player.Vel.x = State->Player.MovementSpeed;
        State->Player.AnimationState = WALKING;
        State->Player.Flags.Flipped = false;
    }

    if(IsKeyUp(KEY_W) && IsKeyUp(KEY_S)) 
    {
        State->Player.Vel.y = 0;
    }

    if(IsKeyUp(KEY_A) && IsKeyUp(KEY_D)) 
    {
        State->Player.Vel.x = 0;
    }

    if(IsKeyUp(KEY_W) & IsKeyUp(KEY_S) & IsKeyUp(KEY_A) & IsKeyUp(KEY_D)) 
    {
        State->Player.AnimationState = IDLE;
    }

    State->Player.Pos.x += State->Player.Vel.x * GetFrameTime();
    State->Player.Pos.y += State->Player.Vel.y * GetFrameTime();

    State->Player.DstRect = 
    {   
        State->Player.Pos.x, 
        State->Player.Pos.y, 
        State->Player.SrcRect.width,
        State->Player.SrcRect.height
    };
}

void 
PlayAnimation(Entity *Entity) 
{
    switch(Entity->EntityType) 
    {
        case PLAYER: 
        {
            switch(Entity->AnimationState) 
            {
                case IDLE: 
                {
                    ++Entity->IdleAnimation.FrameTime;
                    if(Entity->IdleAnimation.FrameTime >= Entity->IdleAnimation.FrameDelay) 
                    {
                        ++Entity->IdleAnimation.CurrentFrame;
                        Entity->IdleAnimation.FrameTime = 0;
                    }

                    if(Entity->IdleAnimation.CurrentFrame > Entity->IdleAnimation.EndingFrame) 
                    {
                        Entity->IdleAnimation.CurrentFrame = Entity->IdleAnimation.StartingFrame;
                    }
                    
                    Entity->SrcRect = 
                    {
                        real32((Entity->Sprite.width / Entity->SpriteFrames) * Entity->IdleAnimation.CurrentFrame),
                        0,
                        real32(Entity->Sprite.width / Entity->SpriteFrames),
                        real32(Entity->Sprite.height),
                    };
                    // TODO : Gross, Maybe find a better way to reset different animation's frames
                    Entity->WalkingAnimation.CurrentFrame = Entity->WalkingAnimation.StartingFrame;
                }break;
                case WALKING:
                {
                    ++Entity->WalkingAnimation.FrameTime;
                    if(Entity->WalkingAnimation.FrameTime > Entity->WalkingAnimation.FrameDelay) 
                    {
                        ++Entity->WalkingAnimation.CurrentFrame;
                        Entity->WalkingAnimation.FrameTime = 0;
                    }

                    if(Entity->WalkingAnimation.CurrentFrame >= Entity->WalkingAnimation.EndingFrame) 
                    {
                        Entity->WalkingAnimation.CurrentFrame = Entity->WalkingAnimation.StartingFrame;
                    }
                    
                    Entity->SrcRect = 
                    {
                        real32((Entity->Sprite.width / Entity->SpriteFrames) * Entity->WalkingAnimation.CurrentFrame),
                        0,
                        real32(Entity->Sprite.width / Entity->SpriteFrames),
                        real32(Entity->Sprite.height),
                    };
                    // TODO : Same as above here, perhaps there's a better way to reset 
                    Entity->IdleAnimation.CurrentFrame = Entity->IdleAnimation.StartingFrame;
                }break;
                case JUMPING:
                {
                }break;
            }
        }break;
        case ENEMY: 
        {
            switch(Entity->AnimationState)
            {
                case IDLE: 
                {
                }break;
                case WALKING:
                {
                }break;
                default: 
                {
                }break;
            }
        }
        case MAPOBJECT: 
        {
            switch(Entity->AnimationState) 
            {
                case IDLE: 
                {
                }break;
                default: 
                {
                }break;
            }
        }break;
    }

    if(Entity->Flags.Flipped) 
    {
        Entity->SrcRect.width = -Entity->SrcRect.width;
        Entity->DstRect.width = -Entity->SrcRect.width;
    }
    else 
    {
        Entity->SrcRect.width = Entity->SrcRect.width;
        Entity->DstRect.width = Entity->SrcRect.width;
    }
}

internal void 
HandleCamera(State *State) 
{
    const int PixelHeightPerScreen = 256;

    State->Camera.zoom = real32(GetScreenHeight() / PixelHeightPerScreen);
    State->Camera.offset = {real32(GetScreenWidth() / 2), real32(GetScreenHeight() / 2)};
    State->Camera.target = {State->Player.Pos.x, State->Player.Pos.y};
}

int main() 
{
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Hello, Window");
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(144);

    State State;    
    InitGameData(&State);
    
    while(!WindowShouldClose())
    {
        BeginDrawing();
        BeginMode2D(State.Camera);
        ClearBackground(BLUE);

        DrawFPS(10, 10);
        HandlePlayerInput(&State);
        HandleCamera(&State);

        State.Player.AnimationState = WALKING;
        State.Player.EntityType = PLAYER;
        PlayAnimation(&State.Player);

        DrawTexturePro
        (
            State.Player.Sprite, 
            State.Player.SrcRect, 
            State.Player.DstRect, 
            {State.Player.DstRect.width / 2, State.Player.DstRect.height}, 
            State.Player.Rotation, 
            WHITE
        );

#ifdef DEBUG
        DrawRectangleV
        (
             State.Player.Pos, 
             {real32(State.Player.Sprite.width / State.Player.SpriteFrames), real32(State.Player.Sprite.height)}, 
             RED
        );
#endif
        EndMode2D();
        EndDrawing();
    }
    CloseWindow();
    return(0);
}
