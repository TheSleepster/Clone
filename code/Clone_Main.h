#pragma once

#include <cstdlib>
#include <cstdint>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <assert.h>
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

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

inline Texture2D 
CreateTextureFromFilepath(const char *Filepath) 
{
    Image Image = LoadImage(Filepath);
    Texture2D Texture = LoadTextureFromImage(Image);

    return(Texture);
}

struct vec2i 
{
    int x;
    int y;
};
