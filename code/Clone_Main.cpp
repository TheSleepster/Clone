#include "Clone_Main.h"

//#define DEBUG

// NOTE : Perhaps we could decide which entity type something is through a simple enum?
// For example, Set the enum to PLAYER to create a PLAYER Entity?
// Perhaps set it to GOBLIN to create a GOBLIN entity?
//
// And yes I know Block Comments exist. Fuck you.

enum AnimationType 
{
    IDLE,
    WALKING,
    JUMPING,
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

struct IdleAnimation
{
    int StartingFrame;
    int EndingFrame;
    int CurrentFrame;
    int FrameTime;
    int FrameDelay;
};

struct WalkingAnimation
{
    int StartingFrame;
    int EndingFrame;
    int CurrentFrame;
    int FrameTime;
    int FrameDelay;
};

struct State 
{
    Camera2D Camera;

    int TextureCount;
    Texture2D *Textures;
    
    struct Entity 
    {
        Texture2D Sprite;
        Rectangle SrcRect;
        Rectangle DstRect;
        bool Flipped;
        bool IsGrounded;

        AnimationType AnimationType;
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

    State->Player.AnimationType = IDLE;
    State->Player.SpriteFrames = 9;
    State->Player.Flipped = false;

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
        State->Player.AnimationType = WALKING;
    }
    else if(IsKeyDown(KEY_S)) 
    {
        State->Player.Vel.y = State->Player.MovementSpeed;
        State->Player.AnimationType = WALKING;
    }

    if(IsKeyDown(KEY_A)) 
    {
        State->Player.Vel.x = -State->Player.MovementSpeed;
        State->Player.AnimationType = WALKING;
        State->Player.Flipped = true;
    }
    else if(IsKeyDown(KEY_D)) 
    {
        State->Player.Vel.x = State->Player.MovementSpeed;
        State->Player.AnimationType = WALKING;
        State->Player.Flipped = false;
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
        State->Player.AnimationType = IDLE;
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

// TODO : Generalize this if we can 
//        (If It isn't already? Idk if it is tbh)
internal void
AnimateSprite(State *State)
{
    switch(State->Player.AnimationType) 
    {
        case IDLE: 
        {
            ++State->Player.IdleAnimation.FrameTime;
            if(State->Player.IdleAnimation.FrameTime >= State->Player.IdleAnimation.FrameDelay) 
            {
                ++State->Player.IdleAnimation.CurrentFrame;
                State->Player.IdleAnimation.FrameTime = 0;
            }

            if(State->Player.IdleAnimation.CurrentFrame > State->Player.IdleAnimation.EndingFrame) 
            {
                State->Player.IdleAnimation.CurrentFrame = State->Player.IdleAnimation.StartingFrame;
            }
            
            State->Player.SrcRect = 
            {
                real32((State->Player.Sprite.width / State->Player.SpriteFrames) * State->Player.IdleAnimation.CurrentFrame),
                0,
                real32(State->Player.Sprite.width / State->Player.SpriteFrames),
                real32(State->Player.Sprite.height),
            };
            // TODO : Gross, Maybe find a better way to reset different animation's frames
            State->Player.WalkingAnimation.CurrentFrame = State->Player.WalkingAnimation.StartingFrame;
        }break;
        case WALKING: 
        {   
            ++State->Player.WalkingAnimation.FrameTime;
            if(State->Player.WalkingAnimation.FrameTime > State->Player.WalkingAnimation.FrameDelay) 
            {
                ++State->Player.WalkingAnimation.CurrentFrame;
                State->Player.WalkingAnimation.FrameTime = 0;
            }

            if(State->Player.WalkingAnimation.CurrentFrame >= State->Player.WalkingAnimation.EndingFrame) 
            {
                State->Player.WalkingAnimation.CurrentFrame = State->Player.WalkingAnimation.StartingFrame;
            }
            
            State->Player.SrcRect = 
            {
                real32((State->Player.Sprite.width / State->Player.SpriteFrames) * State->Player.WalkingAnimation.CurrentFrame),
                0,
                real32(State->Player.Sprite.width / State->Player.SpriteFrames),
                real32(State->Player.Sprite.height),
            };
            // TODO : Same as above here, perhaps there's a better way to reset 
            State->Player.IdleAnimation.CurrentFrame = State->Player.IdleAnimation.StartingFrame;
        }break;
        case JUMPING: 
        {
        }break;
        default: 
        {
        }break;
    }

    if(State->Player.Flipped) 
    {
        State->Player.SrcRect.width = -State->Player.SrcRect.width;
        State->Player.DstRect.width = -State->Player.SrcRect.width;
    }
    else 
    {
        State->Player.SrcRect.width = State->Player.SrcRect.width;
        State->Player.DstRect.width = State->Player.SrcRect.width;
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
        AnimateSprite(&State);

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
