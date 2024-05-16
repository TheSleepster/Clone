#pragma once
#include "Clone_Main.h"
#include "Clone_Data.h"


// NOTE : Fuck you, everything is a box for now.
enum PhysicsBodyShape 
{
    BOX,
};

// TODO : Collision and Mask Layers.

struct PhysicsBody2D 
{
    int BodyID;
    v2 Pos;
    v2 Vel;
    real32 Rotation;
    bool Grounded;
    bool Gravitic;
    bool Enabled;
    
    PhysicsBodyShape PhysicsBodyShape;
};

void CollisionManager(State *State);
