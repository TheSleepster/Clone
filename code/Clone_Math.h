#pragma once
#include "Clone_Intrinsics.h"

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

