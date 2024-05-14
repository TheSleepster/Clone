#pragma once 

#include <cstdlib>
#include <cstdint>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>

#define global_variable static
#define local_persist static
#define internal static

typedef uint8_t  uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uin64;

typedef int8_t  int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

typedef float real32;
typedef double real64;

typedef Vector2 vec2;
typedef Vector3 vec3;
typedef Vector4 vec4;

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

struct vec2i 
{
    int x;
    int y;
};

inline bool 
Vec2EqualsS(vec2 A, real32 B) 
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
Vec2LessThanS(vec2 A, real32 B) 
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
    int SpriteFrames;

    EntityType EntityType;
    AnimationState AnimationState;

    Animation IdleAnimation;
    Animation WalkingAnimation;
    
    real32 MovementSpeed;
    real32 Rotation;
    real32 Scale;
    vec2 Pos, Vel;
};

// NOTE : This way currently doesn't let me place a destructable wall as part of the tileset since the IMAGE
// is what will be drawn as the level. BUT we can just make a hole in the map and fill it with an Entity that
// would be a destructible wall instead. Same for items like Torches, Chests, or Pots.

struct LevelGridData 
{
    vec2i LevelSize;
    char **LevelGrid;
};

struct Level 
{
    Texture2D LevelImage;
    Rectangle SrcRect;
    Rectangle DstRect;
    Entity *Entities;

    LevelGridData LevelGridData;

    bool IsLoaded;
    int Index;
    int EntityCount;

};

// NOTE : Global State

struct State 
{
    int TextureCount;
    int LevelCount;
    Texture2D *Textures;
    Level CurrentLevel;
    
    Entity Player;
    Camera2D Camera;    
    Input Input;
};
