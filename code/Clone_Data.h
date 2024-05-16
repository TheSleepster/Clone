#pragma once
#include "Clone_Main.h"

struct v2i 
{
    int x;
    int y;
};

inline bool 
Vec2EqualsS(v2 A, real32 B) 
{
    bool Result;
    if(A.x == B && A.y == B) 
    {
        Result = true;
    }
    else 
    {
        Result = false;
    }
    return(Result);
}

inline bool 
Vec2LessThanS(v2 A, real32 B) 
{
    bool Result;
    if(A.x >= B) 
    {
        Result = true;
    }
    else 
    {
        Result = false;
    }
    return(Result);
}

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

    struct Jump
    {
        bool Pressed;
        bool Down;
    }Jump;
    
    struct Attack 
    {
        bool Pressed;
        bool Down;
    }Attack;
};

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

// TODO : Potentially create subtypes that indicate what kind of ENEMY or MAPOPJECT something is
enum EntityType 
{
    PLAYER,
    ENEMY,
    MAPOBJECT,
};

struct Animation 
{
    int StartingFrame;
    int EndingFrame;
    int CurrentFrame;
    int FrameTime;
    int FrameDelay;
};    

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
    Rectangle Hitbox;
    int SpriteFrames;

    EntityType EntityType;
    AnimationState AnimationState;

    Animation IdleAnimation;
    Animation WalkingAnimation;
    
    real32 MovementSpeed;
    real32 Rotation;
    real32 Scale;
    v2 Pos, Vel;
};

// NOTE : This way currently doesn't let me place a destructable wall as part of the tileset since the IMAGE
// is what will be drawn as the level. BUT we can just make a hole in the map and fill it with an Entity that
// would be a destructible wall instead. Same for items like Torches, Chests, or Pots.

struct CollisionMap 
{
    v2i MapSize;
    char **CollisionData;
};

struct Level 
{
    Texture2D LevelImage;
    Rectangle SrcRect;
    Rectangle DstRect;
    Entity *Entities;

    CollisionMap CollisionMap;

    bool IsLoaded;
    int Index;
    int EntityCount;
};

// NOTE : Global State

struct State 
{
    int TextureCount;
    int LevelCount;
    real32 Gravity;
    Texture2D *Textures;
    Level CurrentLevel;
    
    Camera2D Camera;    
    Entity Player;
    Input Input;
};
